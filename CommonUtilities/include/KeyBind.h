#pragma once
#include "Enums/EnumKeys.h"

namespace CommonUtilities
{
	struct KeyBind
	{
		eKey myKey;
		eKey myModifier;

		KeyBind();
		KeyBind(eKey aKey, eKey aModifier = eKey::None);
		static bool IsModifierKey(eKey aKey);

	private:
		eKey GetValidModifier(eKey aKey);
	};
}