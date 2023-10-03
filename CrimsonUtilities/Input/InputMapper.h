#pragma once
#include "InputObserver.hpp"
#include "KeyBind.h"
#include "XBoxController.h"
#include "EnumKeys.h"
#include "EnumInputEvents.h"
#include "EnumInputAction.h"
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <vector>

namespace Crimson
{
	class InputMapper
	{
	public:
		~InputMapper();

		void Init(HWND aHandle, bool aUsingXboxInput = false);

		void Attach(InputObserver* anObserver, eInputEvent anEvent, eKey aKey = eKey::None);
		void Attach(InputObserver* anObserver, eInputAction anEvent);

		void BindAction(eInputAction anEvent, KeyBind aKeybind);
		void BindAction(eInputAction anEvent, const std::vector<KeyBind>& aKeybindList);
		void UnbindAction(eInputAction anEvent);

		void Detach(InputObserver* anObserver, eInputEvent anEvent, eKey aKey = eKey::None);
		void Detach(InputObserver* anObserver, eInputAction anEvent);

		void Notify();
		void Update();
		void ResetAll();
		void ResetInput();

		static InputMapper* GetInstance();
		XBoxController* GetXboxController();

		void LockMouse();
		void UnlockMouse();
		void ShowMouse() const;
		void HideMouse() const;
		bool CaptureMouse(bool excludeMenuBar = false);
		bool ReleaseMouse();

		const Vector2<int>& GetMousePosition() const;
		Vector2<int> GetMousePositionInvertedY() const;
		Vector2<int> GetAbsoluteMousePosition() const;
		const Vector2<float>& GetMouseMovement() const;

		bool SetMousePosition(const Vector2<int>& aPosition);
		bool SetMousePosition(const int aX, const int aY);
		bool SetAbsoluteMousePosition(const Vector2<int>& aPosition);
		bool SetAbsoluteMousePosition(const int aX, const int aY);
		bool CenterMouse();

		int GetScrollWheelDelta() const;
		int GetHorizontalScrollWheelDelta() const;

		static bool IsValidCombination(eInputEvent anEvent, eKey aKey);
		static bool IsMouseEvent(eInputEvent anEvent);
		static bool IsMouseButton(eKey aKey);
		static bool IsXboxEvent(eInputEvent anEvent);
		static bool IsXboxButton(eKey aKey);
		static bool IsActionOnly(eKey aKey);

		// Used for manual access to keystates
		bool GetKeyDown(const eKey aKeyCode) const;
		bool GetKeyUp(const eKey aKeyCode) const;
		bool GetKeyHeld(const eKey aKeyCode) const;
		bool GetKeyDownOrHeld(const eKey aKeyCode) const;
		bool GetDoubleMouseClick(const eKey aKeyCode) const;

	private:
		struct KeyObserver
		{
			eKey myKey;
			InputObserver* myObserver;

			bool operator==(const KeyObserver& aComparison) { return myKey == aComparison.myKey && myObserver == aComparison.myObserver; }
		};

		std::unordered_multimap<eInputEvent, KeyObserver> myEventObservers;
		std::unordered_multimap<eInputEvent, eKey> myEvents;

		std::unordered_multimap<eInputAction, InputObserver*> myActionObservers;
		std::unordered_map<eInputAction, std::vector<KeyBind>> myKeybinds;

		std::unordered_multimap<eInputAction, float> myTriggeredActions;
		std::unordered_set<eKey> myObservedKeys;
		std::unordered_set<eKey> myTriggeredKeys;

		std::bitset<256> myCurrentState;
		std::bitset<256> myPreviousState;

		RECT myClientRect;

		Vector2<int> myMousePosition;
		Vector2<int> myScrollDelta;

		Vector2<int> myRelativeCenterPosition;
		Vector2<int> myCenterPosition;

		Vector2<float> myRawMouseMovement;

		HWND myWindowHandle;
		XBoxController* myXboxController;
		static InputMapper* myInstance;

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

		InputMapper();

		friend class InputHandler;
		void UpdateKeyEvent(int aKey, bool aState);
		void UpdateDoubleClick(int aKey);
		void UpdateMouseMove(const Vector2<int>& aPosition);
		void UpdateMouseScroll(int aDelta, bool aIsVertical);
		void UpdateRawData(const Vector2<float>& aMouseDelta);

		void WindowUpdated();
		void WindowLostFocus();
		void WindowGotFocus();

		void CreateEventList();
		void AddMouseEvents();
		void AddMouseActions();
		void AddKeyEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold);
		void AddXboxEvents(bool& aOutTriggeredKeyDown, bool& aOutTriggeredKeyUp, bool& aOutTriggeredKeyHold);
		void AddAction(eKey aKey, float aValue);

		void UpdatePreviousStates();
		void ResetEvents();
		void ReleaseAndResetInput();

		void SendEventNotifications() const;
		void SendActionNotifications() const;
		std::vector<InputObserver*> FindObservers(eInputEvent anEvent, eKey aKey) const;
		std::vector<InputObserver*> FindObservers(eInputAction anEvent) const;

		eKey GetValidKey(eInputEvent anEvent, eKey aKey) const;
		eKey GetXboxKey(const WORD& aWord) const;

		bool IsKeyDown(int aKey) const;
		bool IsKeyUp(int aKey) const;
		bool IsKeyHeld(int aKey) const;
		bool IsKeyReleased(int aKey) const;
	};
}