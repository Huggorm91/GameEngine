#pragma once

namespace CommonUtilities
{
	enum class eInputEvent : int
	{
		MouseMove,
		VerticalScroll,
		HorizontalScroll,
		MouseDoubleClick,
		KeyDown,
		KeyUp,
		KeyHeld,
		XboxLeftStick,
		XboxRightStick,
		XboxLeftTrigger,
		XboxRightTrigger,
		XboxAllInput
	};
}