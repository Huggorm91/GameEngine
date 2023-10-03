#pragma once
#include "EnumKeys.h"

namespace Crimson
{
	struct KeyBind
	{
		eKey myKey;
		eKey myModifier;

		KeyBind();
		KeyBind(eKey aKey, eKey aModifier = eKey::None);

		bool ContainsKey(eKey aKey) const;
		bool ContainsKey(eKey aKey, bool& outIsModifierKey) const;

		bool HasModifier() const;

		static bool IsModifierKey(eKey aKey);

	private:
		eKey GetValidModifier(eKey aKey);
	};
}