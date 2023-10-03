#include "Input.h"

namespace Crimson
{
	Input::Input() : myCurrentState(), myPreviousState(), myDoubleClicks(), myCurrentMousePosition(), myPreviousMousePosition(),
		myCurrentVerticalScrollDelta(), myPreviousVerticalScrollDelta(), myCurrentHorizontalScrollDelta(), myPreviousHorizontalScrollDelta(),
		myWindowHandle(), myIsMouseLocked(false), myCenterPosition()
	{
	}

	Input::Input(HWND aHandle) : myCurrentState(), myPreviousState(), myDoubleClicks(), myCurrentMousePosition(), myPreviousMousePosition(),
		myCurrentVerticalScrollDelta(), myPreviousVerticalScrollDelta(), myCurrentHorizontalScrollDelta(), myPreviousHorizontalScrollDelta(),
		myWindowHandle(aHandle), myIsMouseLocked(false), myCenterPosition()
	{
	}

	void Input::Init(HWND aHandle)
	{
		myWindowHandle = aHandle;
	}

	bool Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (UpdateKeyBoardEvents(message, wParam, lParam))
		{
			return true;
		}
		return UpdateMouseEvents(message, wParam, lParam);
	}

	bool Input::UpdateKeyBoardEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
			case WM_KEYDOWN:
			{
				if (wParam > 256 || wParam < 0)
				{
					return false;
				}
				myCurrentState[wParam] = true;
				return true;
			}
			case WM_KEYUP:
			{
				if (wParam > 256 || wParam < 0)
				{
					return false;
				}
				myCurrentState[wParam] = false;
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	bool Input::UpdateMouseEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_LBUTTONDOWN:
			{
				myCurrentState[static_cast<int>(eKey::MouseLeftButton)] = true;
				return true;
			}
			case WM_LBUTTONUP:
			{
				myCurrentState[static_cast<int>(eKey::MouseLeftButton)] = false;
				return true;
			}
			case WM_LBUTTONDBLCLK:
			{
				myDoubleClicks[static_cast<int>(eKey::MouseLeftButton)] = true;
				return true;
			}
			case WM_RBUTTONDOWN:
			{
				myCurrentState[static_cast<int>(eKey::MouseRightButton)] = true;
				return true;
			}
			case WM_RBUTTONUP:
			{
				myCurrentState[static_cast<int>(eKey::MouseRightButton)] = false;
				return true;
			}
			case WM_RBUTTONDBLCLK:
			{
				myDoubleClicks[static_cast<int>(eKey::MouseRightButton)] = true;
				return true;
			}
			case WM_MBUTTONDOWN:
			{
				myCurrentState[static_cast<int>(eKey::MouseMiddleButton)] = true;
				return true;
			}
			case WM_MBUTTONUP:
			{
				myCurrentState[static_cast<int>(eKey::MouseMiddleButton)] = false;
				return true;
			}
			case WM_MBUTTONDBLCLK:
			{
				myDoubleClicks[static_cast<int>(eKey::MouseMiddleButton)] = true;
				return true;
			}
			case WM_XBUTTONDOWN:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					myCurrentState[static_cast<int>(eKey::MouseXButton1)] = true;
				}
				else
				{
					myCurrentState[static_cast<int>(eKey::MouseXButton2)] = true;
				}
				return true;
			}
			case WM_XBUTTONUP:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					myCurrentState[static_cast<int>(eKey::MouseXButton1)] = false;
				}
				else
				{
					myCurrentState[static_cast<int>(eKey::MouseXButton2)] = false;
				}
				return true;
			}
			case WM_XBUTTONDBLCLK:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					myDoubleClicks[static_cast<int>(eKey::MouseXButton1)] = true;
				}
				else
				{
					myDoubleClicks[static_cast<int>(eKey::MouseXButton2)] = true;
				}
				return true;
			}
			case WM_MOUSEMOVE:
			{
				myCurrentMousePosition = MAKEPOINTS(lParam);
				return true;
			}
			case WM_MOUSEWHEEL:
			{
				if ((myCurrentVerticalScrollDelta < 0) != (GET_WHEEL_DELTA_WPARAM(wParam) < 0))
				{
					myCurrentVerticalScrollDelta = 0;
					myPreviousVerticalScrollDelta = 0;
				}
				myCurrentVerticalScrollDelta += GET_WHEEL_DELTA_WPARAM(wParam);
				return true;
			}
			case WM_MOUSEHWHEEL:
			{
				if ((myCurrentHorizontalScrollDelta < 0) != (GET_WHEEL_DELTA_WPARAM(wParam) < 0))
				{
					myCurrentHorizontalScrollDelta = 0;
					myPreviousHorizontalScrollDelta = 0;
				}
				myCurrentHorizontalScrollDelta += GET_WHEEL_DELTA_WPARAM(wParam);
				return true;
			}
			case WM_EXITSIZEMOVE:
			{
				myCenterPosition.Zero();
				return false;
			}
			default:
			{
				return false;
			}
		}
	}

	bool Input::GetKeyDown(const int aKeyCode) const
	{
		assert(aKeyCode < 256 && aKeyCode >= 0);
		if (aKeyCode >= 256 || aKeyCode < 0)
		{
			return false;
		}

		if (aKeyCode == static_cast<int>(eKey::Any))
		{
			for (size_t index = 0; index < myCurrentState.size(); index++)
			{
				if (myCurrentState[index] == true && myPreviousState[index] == false)
				{
					return true;
				}
			}
			return false;
		}

		return (myCurrentState[aKeyCode] == true && myPreviousState[aKeyCode] == false);
	}

	bool Input::GetKeyUp(const int aKeyCode) const
	{
		assert(aKeyCode < 256 && aKeyCode >= 0);
		if (aKeyCode >= 256 || aKeyCode < 0)
		{
			return false;
		}
		if (aKeyCode == static_cast<int>(eKey::Any))
		{
			for (size_t index = 0; index < myCurrentState.size(); index++)
			{
				if (myCurrentState[index] == false && myPreviousState[index] == true)
				{
					return true;
				}
			}
			return false;
		}

		return (myCurrentState[aKeyCode] == false && myPreviousState[aKeyCode] == true);
	}

	bool Input::GetKeyPress(const int aKeyCode) const
	{
		assert(aKeyCode < 256 && aKeyCode >= 0);
		if (aKeyCode >= 256 || aKeyCode < 0)
		{
			return false;
		}
		if (aKeyCode == static_cast<int>(eKey::Any))
		{
			for (size_t index = 0; index < myCurrentState.size(); index++)
			{
				if (myCurrentState[index] == true && myPreviousState[index] == true)
				{
					return true;
				}
			}
			return false;
		}

		return (myCurrentState[aKeyCode] == true && myPreviousState[aKeyCode] == true);
	}

	bool Input::GetDoubleMouseClick(const int aKeyCode) const
	{
		assert(aKeyCode < 8 && aKeyCode >= 0);
		if (aKeyCode >= 8 || aKeyCode < 0)
		{
			return false;
		}
		if (myDoubleClicks[aKeyCode] == true)
		{
			return true;
		}
		return false;
	}

	bool Input::GetKeyDown(const eKey aKeyCode) const
	{
		return GetKeyDown(static_cast<int>(aKeyCode));
	}

	bool Input::GetKeyUp(const eKey aKeyCode) const
	{
		return GetKeyUp(static_cast<int>(aKeyCode));
	}

	bool Input::GetKeyPress(const eKey aKeyCode) const
	{
		return GetKeyPress(static_cast<int>(aKeyCode));
	}

	bool Input::GetDoubleMouseClick(const eKey aKeyCode) const
	{
		return GetDoubleMouseClick(static_cast<int>(aKeyCode));
	}

	Vector2<int> Input::GetMousePosition() const
	{
		return myCurrentMousePosition;
	}

	Vector2<int> Input::GetPreviousMousePosition() const
	{
		return myPreviousMousePosition;
	}

	Vector2<int> Input::GetAbsoluteMousePosition() const
	{
		POINT point;
		GetCursorPos(&point);
		return point;
	}

	Vector2<int> Input::GetMouseMovement() const
	{
		return myCurrentMousePosition - myPreviousMousePosition;
	}

	bool Input::SetMousePosition(const Vector2<int>& aPosition)
	{
		return SetMousePosition(myWindowHandle, aPosition);
	}

	bool Input::SetMousePosition(const int aX, const int aY)
	{
		return SetMousePosition(myWindowHandle, { aX, aY });
	}

	bool Input::SetMousePosition(HWND aHandle, const Vector2<int>& aPosition)
	{
		POINT point = aPosition;
		RECT maxSize;
		GetClientRect(aHandle, &maxSize);

		if (point.x < maxSize.left)
		{
			point.x = maxSize.left;
		}
		else if (point.x > maxSize.right)
		{
			point.x = maxSize.right;
		}

		if (point.y < maxSize.top)
		{
			point.y = maxSize.top;
		}
		else if (point.y > maxSize.bottom)
		{
			point.y = maxSize.bottom;
		}

		ClientToScreen(aHandle, &point);
		return SetCursorPos(point.x, point.y);
	}

	bool Input::SetMousePosition(HWND aHandle, const int aX, const int aY)
	{
		return SetMousePosition(aHandle, { aX, aY });
	}

	bool Input::SetAbsoluteMousePosition(const Vector2<int>& aPosition)
	{
		return SetCursorPos(aPosition.x, aPosition.y);
	}

	bool Input::SetAbsoluteMousePosition(const int aX, const int aY)
	{
		return SetAbsoluteMousePosition({ aX,aY });
	}

	bool Input::CenterMouse()
	{
		assert(IsWindow(myWindowHandle));
		if (myCenterPosition == Vector2<int>()) 
		{
			RECT rect;
			GetClientRect(myWindowHandle, &rect);

			POINT topLeft;
			topLeft.x = rect.left;
			topLeft.y = rect.top;

			POINT bottomRight;
			bottomRight.x = rect.right;
			bottomRight.y = rect.bottom;

			myCurrentMousePosition = Vector2<int>(bottomRight) / 2;
			myPreviousMousePosition = myCurrentMousePosition;

			MapWindowPoints(myWindowHandle, nullptr, &topLeft, 1);
			MapWindowPoints(myWindowHandle, nullptr, &bottomRight, 1);

			myCenterPosition.x = static_cast<int>((bottomRight.x + topLeft.x) * 0.5f);
			myCenterPosition.y = static_cast<int>((bottomRight.y + topLeft.y) * 0.5f);
		}		

		return SetCursorPos(myCenterPosition.x, myCenterPosition.y);
	}

	void Input::LockMouse()
	{
		myIsMouseLocked = true;
		CenterMouse();
	}

	void Input::UnlockMouse()
	{
		myIsMouseLocked = false;
	}

	bool Input::CaptureMouse(bool excludeMenuBar)
	{
		assert(IsWindow(myWindowHandle));
		if (IsWindow(myWindowHandle) == false)
		{
			return false;
		}
		return CaptureMouse(myWindowHandle, excludeMenuBar);
	}

	bool Input::CaptureMouse(HWND aHandle, bool excludeMenuBar)
	{
		RECT rect;
		if (excludeMenuBar)
		{
			GetClientRect(aHandle, &rect);

			POINT upperLeft;
			upperLeft.x = rect.left;
			upperLeft.y = rect.top;

			POINT lowerRight;
			lowerRight.x = rect.right;
			lowerRight.y = rect.bottom;

			MapWindowPoints(aHandle, nullptr, &upperLeft, 1);
			MapWindowPoints(aHandle, nullptr, &lowerRight, 1);

			rect.left = upperLeft.x;
			rect.top = upperLeft.y;

			rect.right = lowerRight.x;
			rect.bottom = lowerRight.y;
		}
		else
		{
			GetWindowRect(aHandle, &rect);
		}

		return ClipCursor(&rect);
	}

	bool Input::ReleaseMouse() const
	{
		return ClipCursor(nullptr);
	}

	void Input::ShowMouse() const
	{
		ShowCursor(true);
	}

	void Input::HideMouse() const
	{
		ShowCursor(false);
	}

	int Input::GetScrollWheelDelta() const
	{
		return myCurrentVerticalScrollDelta;
	}

	int Input::GetScrollWheelDeltaDifference() const
	{
		return myCurrentVerticalScrollDelta - myPreviousVerticalScrollDelta;
	}

	int Input::GetHorizontalScrollWheelDelta() const
	{
		return myCurrentHorizontalScrollDelta;
	}

	int Input::GetHorizontalScrollWheelDeltaDifference() const
	{
		return myCurrentHorizontalScrollDelta - myPreviousHorizontalScrollDelta;
	}

	void Input::UpdateInput()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
	}

	void Input::UpdateKeyboardInput()
	{
		myPreviousState = myCurrentState;
	}

	void Input::UpdateMouseInput()
	{		
		myPreviousVerticalScrollDelta = myCurrentVerticalScrollDelta;
		myPreviousHorizontalScrollDelta = myCurrentHorizontalScrollDelta;
		myDoubleClicks.reset();
		if (myIsMouseLocked)
		{
			if (myCurrentMousePosition != myPreviousMousePosition)
			{
				CenterMouse();
				myCurrentMousePosition = myPreviousMousePosition;
			}
		}
		else
		{
			myPreviousMousePosition = myCurrentMousePosition;
		}
	}
}