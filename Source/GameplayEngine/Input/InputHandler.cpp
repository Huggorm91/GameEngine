#include "InputHandler.h"
#include "EnumKeys.h"
#include "InputMapper.h"

namespace Crimson
{
	InputHandler::InputHandler(InputMapper& anInputMapper) : myInputMapper(anInputMapper)
	{
	}

	void InputHandler::CreateKeyEvent(int aKey, bool aState)
	{
		myInputMapper.UpdateKeyEvent(aKey, aState);
	}

	void InputHandler::CreateDoubleClickEvent(int aKey)
	{
		myInputMapper.UpdateDoubleClick(aKey);
	}

	void InputHandler::CreateMouseMoveEvent(const Vector2<int>& aPosition)
	{
		myInputMapper.UpdateMouseMove(aPosition);
	}

	void InputHandler::CreateMouseScrollEvent(int aDelta, bool aIsVertical)
	{
		myInputMapper.UpdateMouseScroll(aDelta, aIsVertical);
	}

	void InputHandler::CreateRawDataEvent(RAWINPUT* someRawData)
	{
		if (someRawData->header.dwType == RIM_TYPEMOUSE)
		{
			myInputMapper.UpdateRawData({ static_cast<float>(someRawData->data.mouse.lLastX), static_cast<float>(someRawData->data.mouse.lLastY) });
		}
	}

	bool InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (UpdateKeyBoardEvents(message, wParam, lParam))
		{
			return true;
		}
		return UpdateMouseEvents(message, wParam, lParam);
	}

	bool InputHandler::UpdateKeyBoardEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_KEYDOWN:
			{
				if (wParam > 256 || wParam < 0)
				{
					return false;
				}
				CreateKeyEvent(static_cast<int>(wParam), true);
				return true;
			}
			case WM_KEYUP:
			{
				if (wParam > 256 || wParam < 0)
				{
					return false;
				}
				CreateKeyEvent(static_cast<int>(wParam), false);
				return true;
			}
			default:
			{
				return false;
			}
		}
		lParam;
	}

	bool InputHandler::UpdateMouseEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_LBUTTONDOWN:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseLeftButton), true);
				return true;
			}
			case WM_LBUTTONUP:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseLeftButton), false);
				return true;
			}
			case WM_LBUTTONDBLCLK:
			{
				CreateDoubleClickEvent(static_cast<int>(eKey::MouseLeftButton));
				return true;
			}
			case WM_RBUTTONDOWN:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseRightButton), true);
				return true;
			}
			case WM_RBUTTONUP:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseRightButton), false);
				return true;
			}
			case WM_RBUTTONDBLCLK:
			{
				CreateDoubleClickEvent(static_cast<int>(eKey::MouseRightButton));
				return true;
			}
			case WM_MBUTTONDOWN:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseMiddleButton), true);
				return true;
			}
			case WM_MBUTTONUP:
			{
				CreateKeyEvent(static_cast<int>(eKey::MouseMiddleButton), false);
				return true;
			}
			case WM_MBUTTONDBLCLK:
			{
				CreateDoubleClickEvent(static_cast<int>(eKey::MouseMiddleButton));
				return true;
			}
			case WM_XBUTTONDOWN:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					CreateKeyEvent(static_cast<int>(eKey::MouseXButton1), true);
				}
				else
				{
					CreateKeyEvent(static_cast<int>(eKey::MouseXButton2), true);
				}
				return true;
			}
			case WM_XBUTTONUP:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					CreateKeyEvent(static_cast<int>(eKey::MouseXButton1), false);
				}
				else
				{
					CreateKeyEvent(static_cast<int>(eKey::MouseXButton2), false);
				}
				return true;
			}
			case WM_XBUTTONDBLCLK:
			{
				if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				{
					CreateDoubleClickEvent(static_cast<int>(eKey::MouseXButton1));
				}
				else
				{
					CreateDoubleClickEvent(static_cast<int>(eKey::MouseXButton2));
				}
				return true;
			}
			case WM_MOUSEMOVE:
			{
				CreateMouseMoveEvent(MAKEPOINTS(lParam));
				return true;
			}
			case WM_MOUSEWHEEL:
			{
				CreateMouseScrollEvent(GET_WHEEL_DELTA_WPARAM(wParam), true);
				return true;
			}
			case WM_MOUSEHWHEEL:
			{
				CreateMouseScrollEvent(GET_WHEEL_DELTA_WPARAM(wParam), false);
				return true;
			}
			case WM_EXITSIZEMOVE:
			{
				myInputMapper.WindowUpdated();
				return false;
			}
			case WM_SIZE:
			{
				myInputMapper.WindowUpdated();
				return false;
			}
			/*case WM_KILLFOCUS:
			{
				myInputMapper.WindowLostFocus();
				return false;
			}*/
			case WM_SETFOCUS:
			{
				myInputMapper.WindowGotFocus();
				return false;
			}
			case WM_INPUT:
			{
				UINT dwSize = sizeof(RAWINPUT);
				static BYTE lpb[sizeof(RAWINPUT)];

				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = (RAWINPUT*)lpb;
				CreateRawDataEvent(raw);
				return true;
			}
			default:
			{
				break;
			}
		}

		if (message > 0)
		{
			if (message == WM_SHOWMOUSE)
			{
				if (wParam)
				{
					while (ShowCursor(true) < 0);
				}
				else
				{
					while (ShowCursor(false) >= 0);
				}
				return true;
			}
		}

		return false;
	}
}