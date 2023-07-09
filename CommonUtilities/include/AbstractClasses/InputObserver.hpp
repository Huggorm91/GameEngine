#pragma once
#include "../Enums/EnumKeys.h"
#include "../Enums/EnumInputEvents.h"
#include "../Enums/EnumInputAction.h"

namespace CommonUtilities
{
	class InputObserver
	{
	public:
		virtual ~InputObserver() = default;

		virtual void ReceiveEvent(eInputEvent, eKey) = 0;
		virtual void ReceiveEvent(eInputAction, float) = 0;

	private:
	};
}