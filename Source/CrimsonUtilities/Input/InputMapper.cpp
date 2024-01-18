#include "InputMapper.h"
#include "InputHandler.h"

Crimson::InputMapper::InputMapper() :
	myXboxController(nullptr),
	myClientRect{ 0, 0, 0, 0 },
	myWindowHandle(NULL)
{
}

Crimson::InputMapper::~InputMapper()
{
	if (myXboxController)
	{
		delete myXboxController;
	}
}

void Crimson::InputMapper::Init(HWND aHandle, bool aUsingXboxInput)
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

void Crimson::InputMapper::Attach(InputObserver* anObserver, eInputEvent anEvent, eKey aKey)
{
	assert(anObserver);

	if (myFlags[eFlag::usingXboxInput] == false)
	{
		assert(IsXboxEvent(anEvent) == false && IsXboxButton(aKey) == false);
	}
	if (IsValidCombination(anEvent, aKey))
	{
		if (aKey != eKey::None)
		{
			myObservedKeys.emplace(aKey);
		}
		myEventObservers.emplace(anEvent, EventObserver{ GetValidKey(anEvent, aKey), anObserver });
	}
}

void Crimson::InputMapper::Attach(InputObserver* anObserver, eInputAction anEvent, eKeyAction aKeyAction)
{
	assert(anObserver);

	myActionObservers.emplace(anEvent, ActionObserver{ aKeyAction, anObserver });
}

void Crimson::InputMapper::BindAction(eInputAction anEvent, KeyBind aKeybind)
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
		myKeybinds.emplace(anEvent, std::vector<KeyBind>{aKeybind});
	}
}

void Crimson::InputMapper::BindAction(eInputAction anEvent, const std::vector<KeyBind>& aKeybindList)
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

void Crimson::InputMapper::UnbindAction(eInputAction anEvent)
{
	myKeybinds.erase(anEvent);
}

void Crimson::InputMapper::Detach(InputObserver* anObserver, eInputEvent anEvent, eKey aKey)
{
	eKey comparison = GetValidKey(anEvent, aKey);

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

void Crimson::InputMapper::Detach(InputObserver* anObserver, eInputAction anEvent, eKeyAction aKeyAction)
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

void Crimson::InputMapper::DetachAll(InputObserver* anObserver)
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

void Crimson::InputMapper::Notify()
{
	CreateEventList();
	SendEventNotifications();
	SendActionNotifications();
	ResetEvents();
}

void Crimson::InputMapper::Update()
{
	UpdatePreviousStates();
	if (myFlags[eFlag::usingXboxInput])
	{
		myXboxController->UpdateState();
	}
}

void Crimson::InputMapper::ResetAll()
{
	ResetInput();
	myEventObservers.clear();
	myActionObservers.clear();
	myObservedKeys.clear();
	myKeybinds.clear();
	myFlags[eFlag::isMouseLocked] = false;
	myFlags[hasSetCenterPosition] = false;
	myCenterPosition = Vector2i::Null;
	myRelativeCenterPosition = Vector2i::Null;
}

void Crimson::InputMapper::ResetInput()
{
	ResetEvents();
	myTriggeredKeys.clear();
	myCurrentState.reset();
	myPreviousState.reset();
	myMousePosition = Vector2i::Null;
	myRawMouseMovement = Vector2f::Null;
	myScrollDelta = Vector2i::Null;
	myDoubleClicks.reset();
}

Crimson::XBoxController& Crimson::InputMapper::GetXboxController()
{
	return *myXboxController;
}

void Crimson::InputMapper::LockMouse()
{
	myFlags[eFlag::isMouseLocked] = true;
	CenterMouse();
}

void Crimson::InputMapper::UnlockMouse()
{
	myFlags[eFlag::isMouseLocked] = false;
	myFlags[hasSetCenterPosition] = false;
}

void Crimson::InputMapper::ShowMouse() const
{
	PostMessage(myWindowHandle, WM_SHOWMOUSE, true, 0);
}

void Crimson::InputMapper::HideMouse() const
{
	PostMessage(myWindowHandle, WM_SHOWMOUSE, false, 0);
}

bool Crimson::InputMapper::CaptureMouse(bool excludeMenuBar)
{
	assert(IsWindow(myWindowHandle));

	RECT rect;
	if (excludeMenuBar)
	{
		POINT upperLeft;
		upperLeft.x = myClientRect.left;
		upperLeft.y = myClientRect.top;

		POINT lowerRight;
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

bool Crimson::InputMapper::ReleaseMouse()
{
	return ClipCursor(nullptr);
}

const Crimson::Vector2i& Crimson::InputMapper::GetMousePosition() const
{
	return myMousePosition;
}

Crimson::Vector2i Crimson::InputMapper::GetMousePositionInvertedY() const
{
	return Vector2i(myMousePosition.x, myClientRect.bottom - myMousePosition.y);
}

Crimson::Vector2i Crimson::InputMapper::GetAbsoluteMousePosition() const
{
	POINT point;
	GetCursorPos(&point);
	return point;
}

const Crimson::Vector2f& Crimson::InputMapper::GetMouseMovement() const
{
	return myRawMouseMovement;
}

bool Crimson::InputMapper::SetMousePosition(const Vector2i& aPosition)
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

bool Crimson::InputMapper::SetMousePosition(const int aX, const int aY)
{
	return SetMousePosition({ aX, aY });
}

bool Crimson::InputMapper::SetAbsoluteMousePosition(const Vector2i& aPosition)
{
	return SetCursorPos(aPosition.x, aPosition.y);
}

bool Crimson::InputMapper::SetAbsoluteMousePosition(const int aX, const int aY)
{
	return SetCursorPos(aX, aY);
}

bool Crimson::InputMapper::CenterMouse()
{
	assert(IsWindow(myWindowHandle));
	if (myFlags[hasSetCenterPosition] == false)
	{
		POINT topLeft;
		topLeft.x = myClientRect.left;
		topLeft.y = myClientRect.top;

		POINT bottomRight;
		bottomRight.x = myClientRect.right;
		bottomRight.y = myClientRect.bottom;

		myRelativeCenterPosition = Vector2<int>(bottomRight) / 2;
		myMousePosition = myRelativeCenterPosition;

		MapWindowPoints(myWindowHandle, nullptr, &topLeft, 1);
		MapWindowPoints(myWindowHandle, nullptr, &bottomRight, 1);

		myCenterPosition.x = static_cast<int>((bottomRight.x + topLeft.x) * 0.5f);
		myCenterPosition.y = static_cast<int>((bottomRight.y + topLeft.y) * 0.5f);

		myFlags[hasSetCenterPosition] = true;
	}

	return SetCursorPos(myCenterPosition.x, myCenterPosition.y);
}

int Crimson::InputMapper::GetScrollWheelDelta() const
{
	return myScrollDelta.y;
}

int Crimson::InputMapper::GetHorizontalScrollWheelDelta() const
{
	return myScrollDelta.x;
}

bool Crimson::InputMapper::IsValidCombination(eInputEvent anEvent, eKey aKey)
{
	if (anEvent == eInputEvent::MouseDoubleClick)
	{
		if (!IsMouseButton(aKey))
		{
			return false;
		}
	}
	else if (IsMouseEvent(anEvent) || IsXboxEvent(anEvent))
	{
		if (aKey != eKey::None)
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

bool Crimson::InputMapper::IsMouseEvent(eInputEvent anEvent)
{
	switch (anEvent)
	{
	case eInputEvent::MouseMove:
	case eInputEvent::VerticalScroll:
	case eInputEvent::HorizontalScroll:
	case eInputEvent::MouseDoubleClick:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool Crimson::InputMapper::IsMouseButton(eKey aKey)
{
	return static_cast<int>(aKey) >= static_cast<int>(eKey::MouseLeftButton) &&
		static_cast<int>(aKey) <= static_cast<int>(eKey::MouseXButton2) &&
		aKey != eKey::Cancel;
}

bool Crimson::InputMapper::IsXboxEvent(eInputEvent anEvent)
{
	switch (anEvent)
	{
	case eInputEvent::XboxLeftStick:
	case eInputEvent::XboxRightStick:
	case eInputEvent::XboxLeftTrigger:
	case eInputEvent::XboxRightTrigger:
	case eInputEvent::XboxAllInput:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool Crimson::InputMapper::IsXboxButton(eKey aKey)
{
	return static_cast<int>(aKey) >= static_cast<int>(eKey::Gamepad_A) &&
		static_cast<int>(aKey) <= static_cast<int>(eKey::Gamepad_RightThumbStickButton);
}

bool Crimson::InputMapper::IsActionOnly(eKey aKey)
{
	switch (aKey)
	{
	case eKey::None:
	case eKey::Gamepad_LeftTrigger:
	case eKey::Gamepad_RightTrigger:
	case eKey::Gamepad_LeftThumbStickUp:
	case eKey::Gamepad_LeftThumbStickDown:
	case eKey::Gamepad_LeftThumbStickRight:
	case eKey::Gamepad_LeftThumbStickLeft:
	case eKey::Gamepad_RightThumbStickUp:
	case eKey::Gamepad_RightThumbStickDown:
	case eKey::Gamepad_RightThumbStickRight:
	case eKey::Gamepad_RightThumbStickLeft:
	case eKey::MouseMove_Up:
	case eKey::MouseMove_Down:
	case eKey::MouseMove_Left:
	case eKey::MouseMove_Right:
	case eKey::MouseScroll_Up:
	case eKey::MouseScroll_Down:
	case eKey::MouseScroll_Left:
	case eKey::MouseScroll_Right:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool Crimson::InputMapper::GetKeyDown(const eKey aKeyCode) const
{
	return IsKeyDown(static_cast<int>(aKeyCode));
}

bool Crimson::InputMapper::GetKeyUp(const eKey aKeyCode) const
{
	return IsKeyUp(static_cast<int>(aKeyCode));
}

bool Crimson::InputMapper::GetKeyHeld(const eKey aKeyCode) const
{
	return IsKeyHeld(static_cast<int>(aKeyCode));
}

bool Crimson::InputMapper::GetKeyDownOrHeld(const eKey aKeyCode) const
{
	return myCurrentState[static_cast<int>(aKeyCode)];
}

bool Crimson::InputMapper::GetDoubleMouseClick(const eKey aKeyCode) const
{
	if (IsMouseButton(aKeyCode))
	{
		return myDoubleClicks[static_cast<int>(aKeyCode)];
	}
	return false;
}

void Crimson::InputMapper::UpdateKeyEvent(int aKey, bool aState)
{
	myCurrentState[aKey] = aState;
	myTriggeredKeys.emplace(static_cast<eKey>(aKey));
}

void Crimson::InputMapper::UpdateDoubleClick(int aKey)
{
	myDoubleClicks[aKey] = true;
	myTriggeredEvents[static_cast<int>(eInputEvent::MouseDoubleClick)] = true;
}

void Crimson::InputMapper::UpdateMouseMove(const Vector2<int>& aPosition)
{
	myMousePosition = aPosition;
}

void Crimson::InputMapper::UpdateMouseScroll(int aDelta, bool aIsVertical)
{
	int* currentScrollDelta;
	if (aIsVertical)
	{
		currentScrollDelta = &myScrollDelta.y;
		myTriggeredEvents[static_cast<int>(eInputEvent::VerticalScroll)] = true;
	}
	else
	{
		currentScrollDelta = &myScrollDelta.x;
		myTriggeredEvents[static_cast<int>(eInputEvent::HorizontalScroll)] = true;
	}

	if ((*currentScrollDelta < 0) != (aDelta < 0))
	{
		*currentScrollDelta = 0;
	}
	*currentScrollDelta += aDelta;
}

void Crimson::InputMapper::UpdateRawData(const Vector2<float>& aMouseDelta)
{
	myRawMouseMovement += aMouseDelta * 0.3125f; // 125hz / 400 dpi
	myTriggeredEvents[static_cast<int>(eInputEvent::MouseMove)] = true;
}

void Crimson::InputMapper::WindowUpdated()
{
	ReleaseAndResetInput();
	GetClientRect(myWindowHandle, &myClientRect);
}

void Crimson::InputMapper::WindowLostFocus()
{
	ReleaseAndResetInput();
}

void Crimson::InputMapper::WindowGotFocus()
{
	ReleaseAndResetInput();
}

void Crimson::InputMapper::CreateEventList()
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

void Crimson::InputMapper::AddMouseEvents()
{
	for (int index = 0; index < myTriggeredEvents.size(); index++)
	{
		if (myTriggeredEvents[index] == false)
		{
			continue;
		}

		eInputEvent event = static_cast<eInputEvent>(index);
		if (event == eInputEvent::MouseMove)
		{
			if (myRawMouseMovement != Vector2f::Null)
			{
				myEvents.emplace(event, eKey::None);
			}
			else
			{
				myTriggeredEvents[index] = false;
			}
		}
		else if (event == eInputEvent::MouseDoubleClick)
		{
			for (int key = VK_LBUTTON; key < myDoubleClicks.size(); key++)
			{
				if (key == VK_CANCEL)
				{
					continue;
				}
				if (myDoubleClicks[key])
				{
					myEvents.emplace(eInputEvent::MouseDoubleClick, static_cast<eKey>(key));
				}
			}
		}
		else
		{
			myEvents.emplace(event, eKey::None);
		}
	}
}

void Crimson::InputMapper::AddMouseActions()
{
	if (myTriggeredEvents[static_cast<int>(eInputEvent::MouseMove)])
	{
		if (myRawMouseMovement.x < 0.f)
		{
			AddAction(eKey::MouseMove_Left, myRawMouseMovement.x, false);
		}
		else if (myRawMouseMovement.x > 0.f)
		{
			AddAction(eKey::MouseMove_Right, myRawMouseMovement.x, false);
		}

		if (myRawMouseMovement.y < 0.f)
		{
			AddAction(eKey::MouseMove_Up, myRawMouseMovement.y, false);
		}
		else if (myRawMouseMovement.y > 0.f)
		{
			AddAction(eKey::MouseMove_Down, myRawMouseMovement.y, false);
		}
	}

	if (myTriggeredEvents[static_cast<int>(eInputEvent::VerticalScroll)])
	{
		int delta = GetScrollWheelDelta();
		if (delta < 0)
		{
			AddAction(eKey::MouseScroll_Down, static_cast<float>(delta), false);
		}
		else
		{
			AddAction(eKey::MouseScroll_Up, static_cast<float>(delta), false);
		}
	}

	if (myTriggeredEvents[static_cast<int>(eInputEvent::HorizontalScroll)])
	{
		int delta = GetHorizontalScrollWheelDelta();
		if (delta < 0)
		{
			AddAction(eKey::MouseScroll_Left, static_cast<float>(delta), false);
		}
		else
		{
			AddAction(eKey::MouseScroll_Right, static_cast<float>(delta), false);
		}
	}
}

void Crimson::InputMapper::AddKeyEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold)
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
				myEvents.emplace(eInputEvent::KeyHeld, eKey::Any);
				aOutTriggeredKeyHold = true;
			}
			AddAction(*iter, 1.f, true);
			myEvents.emplace(eInputEvent::KeyHeld, *iter);
			iter++;
		}
		else if (IsKeyDown(static_cast<int>(*iter)))
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(eInputEvent::KeyDown, eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			AddAction(*iter, 2.f, true);
			myEvents.emplace(eInputEvent::KeyDown, *iter);
			iter++;
		}
		else if (IsKeyUp(static_cast<int>(*iter)))
		{
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(eInputEvent::KeyUp, eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(*iter, 0.f, true);
			myEvents.emplace(eInputEvent::KeyUp, *iter);
			iter = myTriggeredKeys.erase(iter);
		}
		else // Key was pressed and released during a single frame
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(eInputEvent::KeyDown, eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(eInputEvent::KeyUp, eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(*iter, 2.f, true);
			AddAction(*iter, 0.f, true);
			myEvents.emplace(eInputEvent::KeyDown, *iter);
			myEvents.emplace(eInputEvent::KeyUp, *iter);
			iter = myTriggeredKeys.erase(iter);
		}
	}
}

void Crimson::InputMapper::AddXboxEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold)
{
	auto& keys = myXboxController->GetKeyArray();

	// Create button events
	for (size_t index = 0; index < keys.size(); index++)
	{
		const eKey key = GetXboxKey(keys[index]);

		if (myXboxController->GetButtonHeld(keys[index]))
		{
			if (aOutTriggeredKeyHold == false)
			{
				myEvents.emplace(eInputEvent::KeyHeld, eKey::Any);
				aOutTriggeredKeyHold = true;
			}
			AddAction(key, 1.f, true);
			myEvents.emplace(eInputEvent::KeyHeld, key);
		}
		else if (myXboxController->GetButtonDown(keys[index]))
		{
			if (aOutTriggeredKeyDown == false)
			{
				myEvents.emplace(eInputEvent::KeyDown, eKey::Any);
				aOutTriggeredKeyDown = true;
			}
			AddAction(key, 2.f, true);
			myEvents.emplace(eInputEvent::KeyDown, key);
		}
		else if (myXboxController->GetButtonUp(keys[index]))
		{
			if (aOutTriggeredKeyUp == false)
			{
				myEvents.emplace(eInputEvent::KeyUp, eKey::Any);
				aOutTriggeredKeyUp = true;
			}
			AddAction(key, 0.f, true);
			myEvents.emplace(eInputEvent::KeyUp, key);
		}
	}

	// Create trigger events
	if (myXboxController->GetRightTrigger() > 0.f)
	{
		myEvents.emplace(eInputEvent::XboxRightTrigger, eKey::None);
		AddAction(eKey::Gamepad_RightTrigger, myXboxController->GetRightTrigger(), false);
	}
	if (myXboxController->GetLeftTrigger() > 0.f)
	{
		myEvents.emplace(eInputEvent::XboxLeftTrigger, eKey::None);
		AddAction(eKey::Gamepad_LeftTrigger, myXboxController->GetLeftTrigger(), false);
	}

	// Create right stick events
	const auto& rightStick = myXboxController->GetRightStick();
	if (rightStick != Vector2f::Null)
	{
		myEvents.emplace(eInputEvent::XboxRightStick, eKey::None);
		if (myActionObservers.size() != 0)
		{
			if (rightStick.x > 0.f)
			{
				AddAction(eKey::Gamepad_RightThumbStickRight, rightStick.x, false);
			}
			else
			{
				AddAction(eKey::Gamepad_RightThumbStickLeft, rightStick.x, false);
			}

			if (rightStick.y > 0.f)
			{
				AddAction(eKey::Gamepad_RightThumbStickUp, rightStick.y, false);
			}
			else
			{
				AddAction(eKey::Gamepad_RightThumbStickDown, rightStick.y, false);
			}
		}
	}

	// Create left stick events
	const auto& leftStick = myXboxController->GetLeftStick();
	if (leftStick != Vector2f::Null)
	{
		myEvents.emplace(eInputEvent::XboxLeftStick, eKey::None);
		if (myActionObservers.size() != 0)
		{
			if (leftStick.x > 0.f)
			{
				AddAction(eKey::Gamepad_LeftThumbStickRight, leftStick.x, false);
			}
			else
			{
				AddAction(eKey::Gamepad_LeftThumbStickLeft, leftStick.x, false);
			}

			if (leftStick.y > 0.f)
			{
				AddAction(eKey::Gamepad_LeftThumbStickUp, leftStick.y, false);
			}
			else
			{
				AddAction(eKey::Gamepad_LeftThumbStickDown, leftStick.y, false);
			}
		}
	}
}

void Crimson::InputMapper::AddAction(eKey aKey, float aValue, bool aIsKeyAction)
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

void Crimson::InputMapper::UpdatePreviousStates()
{
	myPreviousState = myCurrentState;
	myScrollDelta = Vector2i::Null;
	myRawMouseMovement = Vector2f::Null;
	myDoubleClicks.reset();

	if (myFlags[eFlag::isMouseLocked] && myMousePosition != myRelativeCenterPosition)
	{
		CenterMouse();
		myMousePosition = myRelativeCenterPosition;
	}
}

void Crimson::InputMapper::ResetEvents()
{
	myEvents.clear();
	myTriggeredEvents.reset();
	myTriggeredActions.clear();
}

void Crimson::InputMapper::ReleaseAndResetInput()
{
	myEvents.clear();
	myTriggeredEvents.reset();
	myTriggeredActions.clear();
	myCurrentState.reset();
	myPreviousState.reset();
	myMousePosition = Vector2i::Null;
	myRawMouseMovement = Vector2f::Null;
	myScrollDelta = Vector2i::Null;
	myDoubleClicks.reset();
	for (auto iter = myTriggeredKeys.begin(); iter != myTriggeredKeys.end(); ++iter)
	{
		AddAction(static_cast<eKey>(*iter), 0.f, true);
		myEvents.emplace(eInputEvent::KeyUp, static_cast<eKey>(*iter));
	}
	myTriggeredKeys.clear();
}

bool Crimson::InputMapper::IsKeyDown(int aKey) const
{
	return (myCurrentState[aKey] == true && myPreviousState[aKey] == false);
}

bool Crimson::InputMapper::IsKeyUp(int aKey) const
{
	return (myCurrentState[aKey] == false && myPreviousState[aKey] == true);
}

bool Crimson::InputMapper::IsKeyHeld(int aKey) const
{
	return (myCurrentState[aKey] == true && myPreviousState[aKey] == true);
}

bool Crimson::InputMapper::IsKeyReleased(int aKey) const
{
	return (myCurrentState[aKey] == false && myPreviousState[aKey] == false);
}

void Crimson::InputMapper::SendEventNotifications() const
{
	auto xboxAllInputObservers = FindObservers(eInputEvent::XboxAllInput, eKey::None);
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

void Crimson::InputMapper::SendActionNotifications() const
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
				case eKeyAction::KeyDown:
				{
					if (iter->second.first < 1.5f)
					{
						continue;
					}
					break;
				}
				case eKeyAction::KeyHeld:
				{
					if (iter->second.first < 0.5f || iter->second.first > 1.5f)
					{
						continue;
					}
					break;
				}
				case eKeyAction::KeyUp:
				{
					if (iter->second.first > 0.5f)
					{
						continue;
					}
					break;
				}
				case eKeyAction::KeyDownOrHeld:
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

std::vector<Crimson::InputObserver*> Crimson::InputMapper::FindObservers(eInputEvent anEvent, eKey aKey) const
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

std::vector<Crimson::InputMapper::ActionObserver> Crimson::InputMapper::FindObservers(eInputAction anEvent) const
{
	std::vector<ActionObserver> result;
	auto range = myActionObservers.equal_range(anEvent);
	for (auto iter = range.first; iter != range.second; iter++)
	{
		result.emplace_back(iter->second);
	}
	return result;
}

Crimson::eKey Crimson::InputMapper::GetValidKey(eInputEvent anEvent, eKey aKey) const
{
	if (anEvent == eInputEvent::MouseDoubleClick)
	{
		if (!IsMouseButton(aKey))
		{
			return eKey::None;
		}
	}
	else if (IsMouseEvent(anEvent) || IsXboxEvent(anEvent))
	{
		return eKey::None;
	}

	return aKey;
}

Crimson::eKey Crimson::InputMapper::GetXboxKey(const WORD& aWord) const
{
	eKey key = eKey::None;
	switch (aWord)
	{
	case XBOX_DPAD_UP:
	{
		key = eKey::Gamepad_DpadUp;
		break;
	}
	case XBOX_DPAD_DOWN:
	{
		key = eKey::Gamepad_DpadDown;
		break;
	}
	case XBOX_DPAD_LEFT:
	{
		key = eKey::Gamepad_DpadLeft;
		break;
	}
	case XBOX_DPAD_RIGHT:
	{
		key = eKey::Gamepad_DpadRight;
		break;
	}
	case XBOX_START:
	{
		key = eKey::Gamepad_Menu;
		break;
	}
	case XBOX_BACK:
	{
		key = eKey::Gamepad_View;
		break;
	}
	case XBOX_L_THUMBSTICK:
	{
		key = eKey::Gamepad_LeftThumbStickButton;
		break;
	}
	case XBOX_R_THUMBSTICK:
	{
		key = eKey::Gamepad_RightThumbStickButton;
		break;
	}
	case XBOX_L_SHOULDER:
	{
		key = eKey::Gamepad_LeftShoulder;
		break;
	}
	case XBOX_R_SHOULDER:
	{
		key = eKey::Gamepad_RightShoulder;
		break;
	}
	case XBOX_A:
	{
		key = eKey::Gamepad_A;
		break;
	}
	case XBOX_B:
	{
		key = eKey::Gamepad_B;
		break;
	}
	case XBOX_X:
	{
		key = eKey::Gamepad_X;
		break;
	}
	case XBOX_Y:
	{
		key = eKey::Gamepad_Y;
		break;
	}
	default:
		break;
	}
	return key;
}