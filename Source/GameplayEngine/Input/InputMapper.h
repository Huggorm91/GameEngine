#pragma once
#include "InputObserver.h"
#include "KeyBind.h"
#include "XBoxController.h"
#include "EnumKeys.h"
#include "EnumInputEvents.h"
#include "EnumInputAction.h"
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <vector>

class InputMapper
{
	// Ensures safe removal of subscribers even if they have been moved in memory
	friend class InputObserver;
	void Attach(InputObserver* anObserver, Crimson::eInputEvent anEvent, Crimson::eKey aKey = Crimson::eKey::None);
	void Attach(InputObserver* anObserver, Crimson::eInputAction anEvent, Crimson::eKeyAction aKeyAction = Crimson::eKeyAction::All);

	void Detach(InputObserver* anObserver, Crimson::eInputEvent anEvent, Crimson::eKey aKey = Crimson::eKey::None);
	void Detach(InputObserver* anObserver, Crimson::eInputAction anEvent, Crimson::eKeyAction aKeyAction = Crimson::eKeyAction::All);
	void DetachAll(InputObserver* anObserver);
public:
	InputMapper();
	~InputMapper();

	void Init(HWND aHandle, bool aUsingXboxInput = false);


	void BindAction(Crimson::eInputAction anEvent, Crimson::KeyBind aKeybind);
	void BindAction(Crimson::eInputAction anEvent, const std::vector<Crimson::KeyBind>& aKeybindList);
	void UnbindAction(Crimson::eInputAction anEvent);


	void Notify();
	void Update();
	void ResetAll();
	void ResetInput();

	XBoxController& GetXboxController();

	void LockMouse();
	void UnlockMouse();
	void ShowMouse() const;
	void HideMouse() const;
	bool CaptureMouse(bool excludeMenuBar = false) const;
	bool ReleaseMouse();

	const Crimson::Vector2i& GetMousePosition() const;
	Crimson::Vector2i GetMousePositionInvertedY() const;
	Crimson::Vector2i GetAbsoluteMousePosition() const;
	const Crimson::Vector2f& GetMouseMovement() const;

	bool SetMousePosition(const Crimson::Vector2i& aPosition) const;
	bool SetMousePosition(const int aX, const int aY) const;
	bool SetAbsoluteMousePosition(const Crimson::Vector2i& aPosition) const;
	bool SetAbsoluteMousePosition(const int aX, const int aY) const;
	bool CenterMouse();

	int GetScrollWheelDelta() const;
	int GetHorizontalScrollWheelDelta() const;

	static bool IsValidCombination(Crimson::eInputEvent anEvent, Crimson::eKey aKey);
	static bool IsMouseEvent(Crimson::eInputEvent anEvent);
	static bool IsMouseButton(Crimson::eKey aKey);
	static bool IsXboxEvent(Crimson::eInputEvent anEvent);
	static bool IsXboxButton(Crimson::eKey aKey);
	static bool IsActionOnly(Crimson::eKey aKey);

	// Used for manual access to keystates
	bool GetKeyDown(const Crimson::eKey aKeyCode) const;
	bool GetKeyUp(const Crimson::eKey aKeyCode) const;
	bool GetKeyHeld(const Crimson::eKey aKeyCode) const;
	bool GetKeyDownOrHeld(const Crimson::eKey aKeyCode) const;
	bool GetDoubleMouseClick(const Crimson::eKey aKeyCode) const;

private:
	struct EventObserver
	{
		Crimson::eKey myKey;
		InputObserver* myObserver;

		bool operator==(const EventObserver& aComparison) {
			return myKey == aComparison.myKey && myObserver == aComparison.myObserver;
		}
	};

	struct ActionObserver
	{
		Crimson::eKeyAction myKeyAction;
		InputObserver* myObserver;

		bool operator==(const ActionObserver& aComparison) {
			return myKeyAction == aComparison.myKeyAction && myObserver == aComparison.myObserver;
		}
	};

	std::unordered_multimap<Crimson::eInputEvent, EventObserver> myEventObservers;
	std::unordered_multimap<Crimson::eInputEvent, Crimson::eKey> myEvents;

	std::unordered_multimap<Crimson::eInputAction, ActionObserver> myActionObservers;
	std::unordered_map<Crimson::eInputAction, std::vector<Crimson::KeyBind>> myKeybinds;

	std::unordered_multimap<Crimson::eInputAction, std::pair<float, bool>> myTriggeredActions;
	std::unordered_set<Crimson::eKey> myObservedKeys;
	std::unordered_set<Crimson::eKey> myTriggeredKeys;

	std::bitset<256> myCurrentState;
	std::bitset<256> myPreviousState;

	RECT myClientRect;

	Crimson::Vector2i myMousePosition;
	Crimson::Vector2i myScrollDelta;

	Crimson::Vector2i myRelativeCenterPosition;
	Crimson::Vector2i myCenterPosition;

	Crimson::Vector2f myRawMouseMovement;

	HWND myWindowHandle;
	XBoxController* myXboxController;

	std::bitset<VK_XBUTTON2> myDoubleClicks;
	std::bitset<4> myTriggeredEvents;

	enum eFlag
	{
		isMouseLocked,
		usingXboxInput,
		hasSetCenterPosition,
		Count
	};
	std::bitset<eFlag::Count> myFlags;

	friend class InputHandler;
	void UpdateKeyEvent(int aKey, bool aState);
	void UpdateDoubleClick(int aKey);
	void UpdateMouseMove(const Crimson::Vector2i& aPosition);
	void UpdateMouseScroll(int aDelta, bool aIsVertical);
	void UpdateRawData(const Crimson::Vector2f& aMouseDelta);

	void WindowUpdated();
	void WindowLostFocus();
	void WindowGotFocus();

	void CreateEventList();
	void AddMouseEvents();
	void AddMouseActions();
	void AddKeyEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold);
	void AddXboxEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold);
	void AddAction(Crimson::eKey aKey, float aValue, bool aIsKeyAction);

	void UpdatePreviousStates();
	void ResetEvents();
	void ReleaseAndResetInput();

	void SendEventNotifications() const;
	void SendActionNotifications() const;
	std::vector<InputObserver*> FindObservers(Crimson::eInputEvent anEvent, Crimson::eKey aKey) const;
	std::vector<ActionObserver> FindObservers(Crimson::eInputAction anEvent) const;

	Crimson::eKey GetValidKey(Crimson::eInputEvent anEvent, Crimson::eKey aKey) const;
	Crimson::eKey GetXboxKey(const WORD& aWord) const;

	bool IsKeyDown(int aKey) const;
	bool IsKeyUp(int aKey) const;
	bool IsKeyHeld(int aKey) const;
	bool IsKeyReleased(int aKey) const;
};