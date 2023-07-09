#pragma once
#include "Vector2.hpp"
#include "Enums/EnumKeys.h"
#include <bitset>

namespace CommonUtilities
{
	class Input
	{
	public:
		Input();
		Input(HWND aHandle);
		~Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		void Init(HWND aHandle);

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
		bool UpdateKeyBoardEvents(UINT message, WPARAM wParam, LPARAM lParam);
		bool UpdateMouseEvents(UINT message, WPARAM wParam, LPARAM lParam);

		bool GetKeyDown(const int aKeyCode) const;
		bool GetKeyUp(const int aKeyCode) const;
		bool GetKeyPress(const int aKeyCode) const;
		bool GetDoubleMouseClick(const int aKeyCode) const;
		bool GetKeyDown(const eKey aKeyCode) const;
		bool GetKeyUp(const eKey aKeyCode) const;
		bool GetKeyPress(const eKey aKeyCode) const;
		bool GetDoubleMouseClick(const eKey aKeyCode) const;

		Vector2<int> GetMousePosition() const;
		Vector2<int> GetPreviousMousePosition() const;
		Vector2<int> GetAbsoluteMousePosition() const;
		Vector2<int> GetMouseMovement() const;

		bool SetMousePosition(const Vector2<int>& aPosition);
		bool SetMousePosition(const int aX, const int aY);
		bool SetMousePosition(HWND aHandle, const Vector2<int>& aPosition);
		bool SetMousePosition(HWND aHandle, const int aX, const int aY);

		bool SetAbsoluteMousePosition(const Vector2<int>& aPosition);
		bool SetAbsoluteMousePosition(const int aX, const int aY);
		bool CenterMouse();

		void LockMouse();
		void UnlockMouse();

		bool CaptureMouse(bool excludeMenuBar = false);
		bool CaptureMouse(HWND aHandle, bool excludeMenuBar = false);
		bool ReleaseMouse() const;

		void ShowMouse() const;
		void HideMouse() const;

		int GetScrollWheelDelta() const;
		int GetScrollWheelDeltaDifference() const;
		int GetHorizontalScrollWheelDelta() const;
		int GetHorizontalScrollWheelDeltaDifference() const;

		void UpdateInput();
		void UpdateKeyboardInput();
		void UpdateMouseInput();
		
	private:
		std::bitset<256> myCurrentState;
		std::bitset<256> myPreviousState;
		HWND myWindowHandle;
		Vector2<int> myCurrentMousePosition;
		Vector2<int> myPreviousMousePosition;
		Vector2<int> myCenterPosition;
		std::bitset<8> myDoubleClicks;
		int myCurrentVerticalScrollDelta;
		int myPreviousVerticalScrollDelta;
		int myCurrentHorizontalScrollDelta;
		int myPreviousHorizontalScrollDelta;
		bool myIsMouseLocked;
	};	
}

