#include "InputMapper.h"
#include "InputHandler.h"

InputMapper::InputMapper() :
	myXboxController(nullptr),
	myClientRect{ 0, 0, 0, 0 },
	myWindowHandle(NULL)
{
}

InputMapper::~InputMapper()
{
	if (myXboxController)
	{
		delete myXboxController;
	}
}

void InputMapper::Init(HWND aHandle, bool aUsingXboxInput)
{
	myWindowHandle = aHandle;
	GetClientRect(myWindowHandle, &myClientRect);

	myFlags[eFlag::usingXboxInput] = aUsingXboxInput;
	if (aUsingXboxInput && myXboxController == nullptr)
	{
		myXboxController = new XBoxController;
		myXboxController->VerifyConnection();
	}

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC	((USHORT) 0x01)
#endif // !HID_USAGE_PAGE_GENERIC
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE	((USHORT) 0x02)
#endif // !HID_USAGE_GENERIC_MOUSE

	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = aHandle;
	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
}

void InputMapper::Attach(InputObserver* anObserver, Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	assert(anObserver);

	if (myFlags[eFlag::usingXboxInput] == false)
	{
		assert(IsXboxEvent(anEvent) == false && IsXboxButton(aKey) == false);
	}

	if (IsValidCombination(anEvent, aKey))
	{
		if (aKey != Crimson::eKey::None)
		{
			myObservedKeys.emplace(aKey);
		}
		myEventObservers.emplace(anEvent, EventObserver{ GetValidKey(anEvent, aKey), anObserver });
	}
}

void InputMapper::Attach(InputObserver* anObserver, Crimson::eInputAction anEvent, Crimson::eKeyAction aKeyAction)
{
	assert(anObserver);

	myActionObservers.emplace(anEvent, ActionObserver{ aKeyAction, anObserver });
}

void InputMapper::BindAction(Crimson::eInputAction anEvent, Crimson::KeyBind aKeybind)
{
	myObservedKeys.emplace(aKeybind.myKey);
	if (aKeybind.HasModifier())
	{
		myObservedKeys.emplace(aKeybind.myModifier);
	}

	if (auto iter = myKeybinds.find(anEvent); iter != myKeybinds.end())
	{
		iter->second.emplace_back(aKeybind);
	}
	else
	{
		myKeybinds.emplace(anEvent, std::vector<Crimson::KeyBind>{aKeybind});
	}
}

void InputMapper::BindAction(Crimson::eInputAction anEvent, const std::vector<Crimson::KeyBind>& aKeybindList)
{
	for (auto& keybind : aKeybindList)
	{
		myObservedKeys.emplace(keybind.myKey);
		if (keybind.HasModifier())
		{
			myObservedKeys.emplace(keybind.myModifier);
		}
	}
	myKeybinds.emplace(anEvent, aKeybindList);
}

void InputMapper::UnbindAction(Crimson::eInputAction anEvent)
{
	myKeybinds.erase(anEvent);
}

void InputMapper::Detach(InputObserver* anObserver, Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	Crimson::eKey comparison = GetValidKey(anEvent, aKey);

	auto range = myEventObservers.equal_range(anEvent);
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (iter->second.myObserver == anObserver && iter->second.myKey == comparison)
		{
			myEventObservers.erase(iter);
			return;
		}
	}
}

void InputMapper::Detach(InputObserver* anObserver, Crimson::eInputAction anEvent, Crimson::eKeyAction aKeyAction)
{
	auto range = myActionObservers.equal_range(anEvent);
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (iter->second.myObserver == anObserver && iter->second.myKeyAction == aKeyAction)
		{
			myActionObservers.erase(iter);
			return;
		}
	}
}

void InputMapper::DetachAll(InputObserver* anObserver)
{
	auto eventIter = myEventObservers.begin();
	while (eventIter != myEventObservers.end())
	{
		if (eventIter->second.myObserver == anObserver)
		{
			eventIter = myEventObservers.erase(eventIter);
		}
		else
		{
			++eventIter;
		}
	}

	auto actionIter = myActionObservers.begin();
	while (actionIter != myActionObservers.end())
	{
		if (actionIter->second.myObserver == anObserver)
		{
			actionIter = myActionObservers.erase(actionIter);
		}
		else
		{
			++actionIter;
		}
	}
}

void InputMapper::Notify()
{
	CreateEventList();
	SendEventNotifications();
	SendActionNotifications();
	ResetEvents();
}

void InputMapper::Update()
{
	UpdatePreviousStates();
	if (myFlags[eFlag::usingXboxInput])
	{
		myXboxController->UpdateState();
	}
}

void InputMapper::ResetAll()
{
	ResetInput();
	myEventObservers.clear();
	myActionObservers.clear();
	myObservedKeys.clear();
	myKeybinds.clear();
	myFlags[eFlag::isMouseLocked] = false;
	myFlags[hasSetCenterPosition] = false;
	myCenterPosition = Crimson::Vector2i::Null;
	myRelativeCenterPosition = Crimson::Vector2i::Null;
}

void InputMapper::ResetInput()
{
	ResetEvents();
	myTriggeredKeys.clear();
	myCurrentState.reset();
	myPreviousState.reset();
	myMousePosition = Crimson::Vector2i::Null;
	myRawMouseMovement = Crimson::Vector2f::Null;
	myScrollDelta = Crimson::Vector2i::Null;
	myDoubleClicks.reset();
}

XBoxController& InputMapper::GetXboxController()
{
	return *myXboxController;
}

void InputMapper::LockMouse()
{
	myFlags[eFlag::isMouseLocked] = true;
	CenterMouse();
}

void InputMapper::UnlockMouse()
{
	myFlags[eFlag::isMouseLocked] = false;
	myFlags[hasSetCenterPosition] = false;
}

void InputMapper::ShowMouse() const
{
	PostMessage(myWindowHandle, WM_SHOWMOUSE, true, 0);
}

void InputMapper::HideMouse() const
{
	PostMessage(myWindowHandle, WM_SHOWMOUSE, false, 0);
}

bool InputMapper::CaptureMouse(bool excludeMenuBar) const
{
	assert(IsWindow(myWindowHandle));

	RECT rect{};
	if (excludeMenuBar)
	{
		POINT upperLeft{};
		upperLeft.x = myClientRect.left;
		upperLeft.y = myClientRect.top;

		POINT lowerRight{};
		lowerRight.x = myClientRect.right;
		lowerRight.y = myClientRect.bottom;

		MapWindowPoints(myWindowHandle, nullptr, &upperLeft, 1);
		MapWindowPoints(myWindowHandle, nullptr, &lowerRight, 1);

		rect.left = upperLeft.x;
		rect.top = upperLeft.y;

		rect.right = lowerRight.x;
		rect.bottom = lowerRight.y;
	}
	else
	{
		GetWindowRect(myWindowHandle, &rect);
	}
	return ClipCursor(&rect);
}

bool InputMapper::ReleaseMouse()
{
	return ClipCursor(nullptr);
}

const Crimson::Vector2i& InputMapper::GetMousePosition() const
{
	return myMousePosition;
}

Crimson::Vector2i InputMapper::GetMousePositionInvertedY() const
{
	return Crimson::Vector2i(myMousePosition.x, myClientRect.bottom - myMousePosition.y);
}

Crimson::Vector2i InputMapper::GetAbsoluteMousePosition() const
{
	POINT point{};
	GetCursorPos(&point);
	return point;
}

const Crimson::Vector2f& InputMapper::GetMouseMovement() const
{
	return myRawMouseMovement;
}

bool InputMapper::SetMousePosition(const Crimson::Vector2i& aPosition) const
{
	assert(IsWindow(myWindowHandle));

	POINT point = aPosition;

	if (point.x < myClientRect.left)
	{
		point.x = myClientRect.left;
	}
	else if (point.x > myClientRect.right)
	{
		point.x = myClientRect.right;
	}

	if (point.y < myClientRect.top)
	{
		point.y = myClientRect.top;
	}
	else if (point.y > myClientRect.bottom)
	{
		point.y = myClientRect.bottom;
	}

	ClientToScreen(myWindowHandle, &point);
	return SetCursorPos(point.x, point.y);
}

bool InputMapper::SetMousePosition(const int aX, const int aY) const
{
	return SetMousePosition({ aX, aY });
}

bool InputMapper::SetAbsoluteMousePosition(const Crimson::Vector2i& aPosition) const
{
	return SetCursorPos(aPosition.x, aPosition.y);
}

bool InputMapper::SetAbsoluteMousePosition(const int aX, const int aY) const
{
	return SetCursorPos(aX, aY);
}

bool InputMapper::CenterMouse()
{
	assert(IsWindow(myWindowHandle));
	if (myFlags[hasSetCenterPosition] == false)
	{
		POINT topLeft{};
		topLeft.x = myClientRect.left;
		topLeft.y = myClientRect.top;

		POINT bottomRight{};
		bottomRight.x = myClientRect.right;
		bottomRight.y = myClientRect.bottom;

		myRelativeCenterPosition = Crimson::Vector2i(bottomRight) / 2;
		myMousePosition = myRelativeCenterPosition;

		MapWindowPoints(myWindowHandle, nullptr, &topLeft, 1);
		MapWindowPoints(myWindowHandle, nullptr, &bottomRight, 1);

		myCenterPosition.x = static_cast<int>((bottomRight.x + topLeft.x) * 0.5f);
		myCenterPosition.y = static_cast<int>((bottomRight.y + topLeft.y) * 0.5f);

		myFlags[hasSetCenterPosition] = true;
	}

	return SetCursorPos(myCenterPosition.x, myCenterPosition.y);
}

int InputMapper::GetScrollWheelDelta() const
{
	return myScrollDelta.y;
}

int InputMapper::GetHorizontalScrollWheelDelta() const
{
	return myScrollDelta.x;
}

bool InputMapper::IsValidCombination(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (anEvent == Crimson::eInputEvent::MouseDoubleClick)
	{
		if (!IsMouseButton(aKey))
		{
			return false;
		}
	}
	else if (IsMouseEvent(anEvent) || IsXboxEvent(anEvent))
	{
		if (aKey != Crimson::eKey::None)
		{
			return false;
		}
	}
	else
	{
		if (IsActionOnly(aKey))
		{
			return false;
		}
	}
	return true;
}

bool InputMapper::IsMouseEvent(Crimson::eInputEvent anEvent)
{
	switch (anEvent)
	{
	case Crimson::eInputEvent::MouseMove:
	case Crimson::eInputEvent::VerticalScroll:
	case Crimson::eInputEvent::HorizontalScroll:
	case Crimson::eInputEvent::MouseDoubleClick:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool InputMapper::IsMouseButton(Crimson::eKey aKey)
{
	return static_cast<int>(aKey) >= static_cast<int>(Crimson::eKey::MouseLeftButton) &&
		static_cast<int>(aKey) <= static_cast<int>(Crimson::eKey::MouseXButton2) &&
		aKey != Crimson::eKey::Cancel;
}

bool InputMapper::IsXboxEvent(Crimson::eInputEvent anEvent)
{
	switch (anEvent)
	{
	case Crimson::eInputEvent::XboxLeftStick:
	case Crimson::eInputEvent::XboxRightStick:
	case Crimson::eInputEvent::XboxLeftTrigger:
	case Crimson::eInputEvent::XboxRightTrigger:
	case Crimson::eInputEvent::XboxAllInput:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool InputMapper::IsXboxButton(Crimson::eKey aKey)
{
	return static_cast<int>(aKey) >= static_cast<int>(Crimson::eKey::Gamepad_A) &&
		static_cast<int>(aKey) <= static_cast<int>(Crimson::eKey::Gamepad_RightThumbStickButton);
}

bool InputMapper::IsActionOnly(Crimson::eKey aKey)
{
	switch (aKey)
	{
	case Crimson::eKey::None:
	case Crimson::eKey::Gamepad_LeftTrigger:
	case Crimson::eKey::Gamepad_RightTrigger:
	case Crimson::eKey::Gamepad_LeftThumbStickUp:
	case Crimson::eKey::Gamepad_LeftThumbStickDown:
	case Crimson::eKey::Gamepad_LeftThumbStickRight:
	case Crimson::eKey::Gamepad_LeftThumbStickLeft:
	case Crimson::eKey::Gamepad_RightThumbStickUp:
	case Crimson::eKey::Gamepad_RightThumbStickDown:
	case Crimson::eKey::Gamepad_RightThumbStickRight:
	case Crimson::eKey::Gamepad_RightThumbStickLeft:
	case Crimson::eKey::MouseMove_Up:
	case Crimson::eKey::MouseMove_Down:
	case Crimson::eKey::MouseMove_Left:
	case Crimson::eKey::MouseMove_Right:
	case Crimson::eKey::MouseScroll_Up:
	case Crimson::eKey::MouseScroll_Down:
	case Crimson::eKey::MouseScroll_Left:
	case Crimson::eKey::MouseScroll_Right:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool InputMapper::GetKeyDown(const Crimson::eKey aKeyCode) const
{
	return IsKeyDown(static_cast<int>(aKeyCode));
}

bool InputMapper::GetKeyUp(const Crimson::eKey aKeyCode) const
{
	return IsKeyUp(static_cast<int>(aKeyCode));
}

bool InputMapper::GetKeyHeld(const Crimson::eKey aKeyCode) const
{
	return IsKeyHeld(static_cast<int>(aKeyCode));
}

bool InputMapper::GetKeyDownOrHeld(const Crimson::eKey aKeyCode) const
{
	return myCurrentState[static_cast<int>(aKeyCode)];
}

bool InputMapper::GetDoubleMouseClick(const Crimson::eKey aKeyCode) const
{
	if (IsMouseButton(aKeyCode))
	{
		return myDoubleClicks[static_cast<int>(aKeyCode)];
	}
	return false;
}

void InputMapper::UpdateKeyEvent(int aKey, bool aState)
{
	myCurrentState[aKey] = aState;
	myTriggeredKeys.emplace(static_cast<Crimson::eKey>(aKey));
}

void InputMapper::UpdateDoubleClick(int aKey)
{
	myDoubleClicks[aKey] = true;
	myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::MouseDoubleClick)] = true;
}

void InputMapper::UpdateMouseMove(const Crimson::Vector2i& aPosition)
{
	myMousePosition = aPosition;
}

void InputMapper::UpdateMouseScroll(int aDelta, bool aIsVertical)
{
	int* currentScrollDelta;
	if (aIsVertical)
	{
		currentScrollDelta = &myScrollDelta.y;
		myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::VerticalScroll)] = true;
	}
	else
	{
		currentScrollDelta = &myScrollDelta.x;
		myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::HorizontalScroll)] = true;
	}

	if ((*currentScrollDelta < 0) != (aDelta < 0))
	{
		*currentScrollDelta = 0;
	}
	*currentScrollDelta += aDelta;
}

void InputMapper::UpdateRawData(const Crimson::Vector2f& aMouseDelta)
{
	myRawMouseMovement += aMouseDelta * 0.3125f; // 125hz / 400 dpi
	myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::MouseMove)] = true;
}

void InputMapper::WindowUpdated()
{
	ReleaseAndResetInput();
	GetClientRect(myWindowHandle, &myClientRect);
}

void InputMapper::WindowLostFocus()
{
	ReleaseAndResetInput();
}

void InputMapper::WindowGotFocus()
{
	ReleaseAndResetInput();
}

void InputMapper::CreateEventList()
{
	AddMouseEvents();
	AddMouseActions();

	// Flags for 'Any' key events
	bool triggeredKeyDown = false;
	bool triggeredKeyUp = false;
	bool triggeredKeyHold = false;

	AddKeyEvents(triggeredKeyDown, triggeredKeyUp, triggeredKeyHold);
	if (myFlags[eFlag::usingXboxInput])
	{
		AddXboxEvents(triggeredKeyDown, triggeredKeyUp, triggeredKeyHold);
	}
}

void InputMapper::AddMouseEvents()
{
	for (int index = 0; index < myTriggeredEvents.size(); index++)
	{
		if (myTriggeredEvents[index] == false)
		{
			continue;
		}

		Crimson::eInputEvent event = static_cast<Crimson::eInputEvent>(index);
		if (event == Crimson::eInputEvent::MouseMove)
		{
			if (myRawMouseMovement != Crimson::Vector2f::Null)
			{
				myEvents.emplace(event, Crimson::eKey::None);
			}
			else
			{
				myTriggeredEvents[index] = false;
			}
		}
		else if (event == Crimson::eInputEvent::MouseDoubleClick)
		{
			for (int key = VK_LBUTTON; key < myDoubleClicks.size(); key++)
			{
				if (key == VK_CANCEL)
				{
					continue;
				}
				if (myDoubleClicks[key])
				{
					myEvents.emplace(Crimson::eInputEvent::MouseDoubleClick, static_cast<Crimson::eKey>(key));
				}
			}
		}
		else
		{
			myEvents.emplace(event, Crimson::eKey::None);
		}
	}
}

void InputMapper::AddMouseActions()
{
	if (myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::MouseMove)])
	{
		if (myRawMouseMovement.x < 0.f)
		{
			AddAction(Crimson::eKey::MouseMove_Left, myRawMouseMovement.x, false);
		}
		else if (myRawMouseMovement.x > 0.f)
		{
			AddAction(Crimson::eKey::MouseMove_Right, myRawMouseMovement.x, false);
		}

		if (myRawMouseMovement.y < 0.f)
		{
			AddAction(Crimson::eKey::MouseMove_Up, myRawMouseMovement.y, false);
		}
		else if (myRawMouseMovement.y > 0.f)
		{
			AddAction(Crimson::eKey::MouseMove_Down, myRawMouseMovement.y, false);
		}
	}

	if (myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::VerticalScroll)])
	{
		int delta = GetScrollWheelDelta();
		if (delta < 0)
		{
			AddAction(Crimson::eKey::MouseScroll_Down, static_cast<float>(delta), false);
		}
		else
		{
			AddAction(Crimson::eKey::MouseScroll_Up, static_cast<float>(delta), false);
		}
	}

	if (myTriggeredEvents[static_cast<int>(Crimson::eInputEvent::HorizontalScroll)])
	{
		int delta = GetHorizontalScrollWheelDelta();
		if (delta < 0)
		{
			AddAction(Crimson::eKey::MouseScroll_Left, static_cast<float>(delta), false);
		}
		else
		{
			AddAction(Crimson::eKey::MouseScroll_Right, static_cast<float>(delta), false);
		}
	}
}

void InputMapper::AddKeyEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold)
{
	for (auto iter = myTriggeredKeys.begin(); iter != myTriggeredKeys.end();)
	{
		if (myObservedKeys.find(*iter) == myObservedKeys.end())
		{
			iter++;
			continue;
		}

		if (IsKeyHeld(static_cast<int>(*iter)))
		{
			if (aOutTriggeredKeyHold == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyHeld, Crimson::eKey::Any);
				aOutTriggeredKeyHold = true;
			}
			AddAction(*iter, 1.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyHeld, *iter);
			iter++;
		}
		else if (IsKeyDown(static_cast<int>(*iter)))
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyDown, Crimson::eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			AddAction(*iter, 2.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyDown, *iter);
			iter++;
		}
		else if (IsKeyUp(static_cast<int>(*iter)))
		{
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyUp, Crimson::eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(*iter, 0.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyUp, *iter);
			iter = myTriggeredKeys.erase(iter);
		}
		else // Key was pressed and released during a single frame
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyDown, Crimson::eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyUp, Crimson::eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(*iter, 2.f, true);
			AddAction(*iter, 0.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyDown, *iter);
			myEvents.emplace(Crimson::eInputEvent::KeyUp, *iter);
			iter = myTriggeredKeys.erase(iter);
		}
	}
}

void InputMapper::AddXboxEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold)
{
	auto& keys = myXboxController->GetKeyArray();

	// Create button events
	for (size_t index = 0; index < keys.size(); index++)
	{
		const Crimson::eKey key = GetXboxKey(keys[index]);

		if (myXboxController->GetButtonHeld(keys[index]))
		{
			if (aOutTriggeredKeyHold == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyHeld, Crimson::eKey::Any);
				aOutTriggeredKeyHold = true;
			}
			AddAction(key, 1.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyHeld, key);
		}
		else if (myXboxController->GetButtonDown(keys[index]))
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyDown, Crimson::eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			AddAction(key, 2.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyDown, key);
		}
		else if (myXboxController->GetButtonUp(keys[index]))
		{
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(Crimson::eInputEvent::KeyUp, Crimson::eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(key, 0.f, true);
			myEvents.emplace(Crimson::eInputEvent::KeyUp, key);
		}
	}

	// Create trigger events
	if (myXboxController->GetRightTrigger() > 0.f)
	{
		myEvents.emplace(Crimson::eInputEvent::XboxRightTrigger, Crimson::eKey::None);
		AddAction(Crimson::eKey::Gamepad_RightTrigger, myXboxController->GetRightTrigger(), false);
	}
	if (myXboxController->GetLeftTrigger() > 0.f)
	{
		myEvents.emplace(Crimson::eInputEvent::XboxLeftTrigger, Crimson::eKey::None);
		AddAction(Crimson::eKey::Gamepad_LeftTrigger, myXboxController->GetLeftTrigger(), false);
	}

	// Create right stick events
	const auto& rightStick = myXboxController->GetRightStick();
	if (rightStick != Crimson::Vector2f::Null)
	{
		myEvents.emplace(Crimson::eInputEvent::XboxRightStick, Crimson::eKey::None);
		if (myActionObservers.size() != 0)
		{
			if (rightStick.x > 0.f)
			{
				AddAction(Crimson::eKey::Gamepad_RightThumbStickRight, rightStick.x, false);
			}
			else
			{
				AddAction(Crimson::eKey::Gamepad_RightThumbStickLeft, rightStick.x, false);
			}

			if (rightStick.y > 0.f)
			{
				AddAction(Crimson::eKey::Gamepad_RightThumbStickUp, rightStick.y, false);
			}
			else
			{
				AddAction(Crimson::eKey::Gamepad_RightThumbStickDown, rightStick.y, false);
			}
		}
	}

	// Create left stick events
	const auto& leftStick = myXboxController->GetLeftStick();
	if (leftStick != Crimson::Vector2f::Null)
	{
		myEvents.emplace(Crimson::eInputEvent::XboxLeftStick, Crimson::eKey::None);
		if (myActionObservers.size() != 0)
		{
			if (leftStick.x > 0.f)
			{
				AddAction(Crimson::eKey::Gamepad_LeftThumbStickRight, leftStick.x, false);
			}
			else
			{
				AddAction(Crimson::eKey::Gamepad_LeftThumbStickLeft, leftStick.x, false);
			}

			if (leftStick.y > 0.f)
			{
				AddAction(Crimson::eKey::Gamepad_LeftThumbStickUp, leftStick.y, false);
			}
			else
			{
				AddAction(Crimson::eKey::Gamepad_LeftThumbStickDown, leftStick.y, false);
			}
		}
	}
}

void InputMapper::AddAction(Crimson::eKey aKey, float aValue, bool aIsKeyAction)
{
	bool isModifierKey;
	for (auto iter = myKeybinds.begin(); iter != myKeybinds.end(); iter++)
	{
		for (auto& keybind : iter->second)
		{
			if (!keybind.ContainsKey(aKey, isModifierKey))
			{
				continue;
			}

			if (aValue == 1.f) // KeyHeld
			{
				if (isModifierKey && !IsKeyHeld(static_cast<int>(keybind.myKey)))
				{
					continue;
				}
				else if (!isModifierKey && keybind.HasModifier() && !IsKeyHeld(static_cast<int>(keybind.myModifier)))
				{
					continue;
				}
			}
			else // KeyDown & KeyUp
			{
				if (isModifierKey)
				{
					if (myTriggeredKeys.find(keybind.myKey) == myTriggeredKeys.end())
					{
						continue;
					}
				}
				else if (keybind.HasModifier())
				{
					if (myTriggeredKeys.find(keybind.myModifier) == myTriggeredKeys.end())
					{
						continue;
					}
				}
			}

			// Check if an event with the same value already has been triggered
			if (const auto& range = myTriggeredActions.equal_range(iter->first); range.first != myTriggeredActions.end())
			{
				bool hasTriggered = false;
				for (auto actionIter = range.first; actionIter != range.second; actionIter++)
				{
					if (actionIter->second.first == aValue)
					{
						hasTriggered = true;
						break;
					}
				}

				if (hasTriggered)
				{
					continue;
				}
			}

			myTriggeredActions.emplace(iter->first, std::pair<float, bool>(aValue, aIsKeyAction));
		}
	}
}

void InputMapper::UpdatePreviousStates()
{
	myPreviousState = myCurrentState;
	myScrollDelta = Crimson::Vector2i::Null;
	myRawMouseMovement = Crimson::Vector2f::Null;
	myDoubleClicks.reset();

	if (myFlags[eFlag::isMouseLocked] && myMousePosition != myRelativeCenterPosition)
	{
		CenterMouse();
		myMousePosition = myRelativeCenterPosition;
	}
}

void InputMapper::ResetEvents()
{
	myEvents.clear();
	myTriggeredEvents.reset();
	myTriggeredActions.clear();
}

void InputMapper::ReleaseAndResetInput()
{
	myEvents.clear();
	myTriggeredEvents.reset();
	myTriggeredActions.clear();
	myCurrentState.reset();
	myPreviousState.reset();
	myMousePosition = Crimson::Vector2i::Null;
	myRawMouseMovement = Crimson::Vector2f::Null;
	myScrollDelta = Crimson::Vector2i::Null;
	myDoubleClicks.reset();
	for (auto iter = myTriggeredKeys.begin(); iter != myTriggeredKeys.end(); ++iter)
	{
		AddAction(static_cast<Crimson::eKey>(*iter), 0.f, true);
		myEvents.emplace(Crimson::eInputEvent::KeyUp, static_cast<Crimson::eKey>(*iter));
	}
	myTriggeredKeys.clear();
}

bool InputMapper::IsKeyDown(int aKey) const
{
	return (myCurrentState[aKey] == true && myPreviousState[aKey] == false);
}

bool InputMapper::IsKeyUp(int aKey) const
{
	return (myCurrentState[aKey] == false && myPreviousState[aKey] == true);
}

bool InputMapper::IsKeyHeld(int aKey) const
{
	return (myCurrentState[aKey] == true && myPreviousState[aKey] == true);
}

bool InputMapper::IsKeyReleased(int aKey) const
{
	return (myCurrentState[aKey] == false && myPreviousState[aKey] == false);
}

void InputMapper::SendEventNotifications() const
{
	auto xboxAllInputObservers = FindObservers(Crimson::eInputEvent::XboxAllInput, Crimson::eKey::None);
	for (auto iter = myEvents.begin(); iter != myEvents.end(); iter++)
	{
		auto observerList = FindObservers(iter->first, iter->second);
		for (auto& observer : observerList)
		{
			observer->ReceiveEvent(iter->first, iter->second);
		}

		if (IsXboxEvent(iter->first) || IsXboxButton(iter->second))
		{
			for (auto& observer : xboxAllInputObservers)
			{
				observer->ReceiveEvent(iter->first, iter->second);
			}
		}
	}
}

void InputMapper::SendActionNotifications() const
{
	for (auto iter = myTriggeredActions.begin(); iter != myTriggeredActions.end(); iter++)
	{
		auto observerList = FindObservers(iter->first);
		for (auto& observer : observerList)
		{
			if (iter->second.second)
			{
				switch (observer.myKeyAction)
				{
				case Crimson::eKeyAction::KeyDown:
				{
					if (iter->second.first < 1.5f)
					{
						continue;
					}
					break;
				}
				case Crimson::eKeyAction::KeyHeld:
				{
					if (iter->second.first < 0.5f || iter->second.first > 1.5f)
					{
						continue;
					}
					break;
				}
				case Crimson::eKeyAction::KeyUp:
				{
					if (iter->second.first > 0.5f)
					{
						continue;
					}
					break;
				}
				case Crimson::eKeyAction::KeyDownOrHeld:
				{
					if (iter->second.first < 0.5f)
					{
						continue;
					}
					break;
				}
				default:
					break;
				}
			}

			observer.myObserver->ReceiveEvent(iter->first, iter->second.first);
		}
	}
}

std::vector<InputObserver*> InputMapper::FindObservers(Crimson::eInputEvent anEvent, Crimson::eKey aKey) const
{
	std::vector<InputObserver*> result;
	auto range = myEventObservers.equal_range(anEvent);
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (iter->second.myKey == aKey)
		{
			result.emplace_back(iter->second.myObserver);
		}
	}
	return result;
}

std::vector<InputMapper::ActionObserver> InputMapper::FindObservers(Crimson::eInputAction anEvent) const
{
	std::vector<ActionObserver> result;
	auto range = myActionObservers.equal_range(anEvent);
	for (auto iter = range.first; iter != range.second; iter++)
	{
		result.emplace_back(iter->second);
	}
	return result;
}

Crimson::eKey InputMapper::GetValidKey(Crimson::eInputEvent anEvent, Crimson::eKey aKey) const
{
	if (anEvent == Crimson::eInputEvent::MouseDoubleClick)
	{
		if (!IsMouseButton(aKey))
		{
			return Crimson::eKey::None;
		}
	}
	else if (IsMouseEvent(anEvent) || IsXboxEvent(anEvent))
	{
		return Crimson::eKey::None;
	}

	return aKey;
}

Crimson::eKey InputMapper::GetXboxKey(const WORD& aWord) const
{
	Crimson::eKey key = Crimson::eKey::None;
	switch (aWord)
	{
	case XBOX_DPAD_UP:
	{
		key = Crimson::eKey::Gamepad_DpadUp;
		break;
	}
	case XBOX_DPAD_DOWN:
	{
		key = Crimson::eKey::Gamepad_DpadDown;
		break;
	}
	case XBOX_DPAD_LEFT:
	{
		key = Crimson::eKey::Gamepad_DpadLeft;
		break;
	}
	case XBOX_DPAD_RIGHT:
	{
		key = Crimson::eKey::Gamepad_DpadRight;
		break;
	}
	case XBOX_START:
	{
		key = Crimson::eKey::Gamepad_Menu;
		break;
	}
	case XBOX_BACK:
	{
		key = Crimson::eKey::Gamepad_View;
		break;
	}
	case XBOX_L_THUMBSTICK:
	{
		key = Crimson::eKey::Gamepad_LeftThumbStickButton;
		break;
	}
	case XBOX_R_THUMBSTICK:
	{
		key = Crimson::eKey::Gamepad_RightThumbStickButton;
		break;
	}
	case XBOX_L_SHOULDER:
	{
		key = Crimson::eKey::Gamepad_LeftShoulder;
		break;
	}
	case XBOX_R_SHOULDER:
	{
		key = Crimson::eKey::Gamepad_RightShoulder;
		break;
	}
	case XBOX_A:
	{
		key = Crimson::eKey::Gamepad_A;
		break;
	}
	case XBOX_B:
	{
		key = Crimson::eKey::Gamepad_B;
		break;
	}
	case XBOX_X:
	{
		key = Crimson::eKey::Gamepad_X;
		break;
	}
	case XBOX_Y:
	{
		key = Crimson::eKey::Gamepad_Y;
		break;
	}
	default:
		break;
	}
	return key;
}