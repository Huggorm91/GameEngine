#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"

namespace Crimson
{
	const unsigned int WM_SHOWMOUSE = RegisterWindowMessage(L"ShowMouse");

	class InputMapper;

	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler() = default;
		InputHandler(const InputHandler&) = delete;
		InputHandler& operator=(const InputHandler&) = delete;

		void CreateKeyEvent(int aKey, bool aState);
		void CreateDoubleClickEvent(int aKey);
		void CreateMouseMoveEvent(const Vector2<int>& aPosition);
		void CreateMouseScrollEvent(int aDelta, bool aIsVertical);
		void CreateRawDataEvent(RAWINPUT* someRawData);

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
		bool UpdateKeyBoardEvents(UINT message, WPARAM wParam, LPARAM lParam);
		bool UpdateMouseEvents(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		InputMapper& myInputMapper;
	};	
}