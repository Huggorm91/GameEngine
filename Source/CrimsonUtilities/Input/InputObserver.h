#pragma once
#include "EnumKeys.h"
#include "EnumInputEvents.h"
#include "EnumInputAction.h"

namespace Crimson
{
	class InputObserver
	{
	public:
		virtual ~InputObserver();

		virtual void ReceiveEvent(eInputEvent, eKey){}

		// float value will be different depending on what Keybind was triggered.
		// Key		| KeyUp = 0.f	:	KeyHeld = 1.f	:	KeyDown = 2.f
		// Xbox		| Sticks = 0.f to 1.f	:	Trigger = 0.f to 1.f
		// Mouse	| Move = Pixeldistance moved	:	Scroll = Delta
		virtual void ReceiveEvent(eInputAction, float){}

	private:
	};
}