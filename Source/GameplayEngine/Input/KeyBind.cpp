#include "KeyBind.h"

Crimson::KeyBind::KeyBind() : myKey(eKey::None), myModifier(eKey::None)
{
}

Crimson::KeyBind::KeyBind(eKey aKey, eKey aModifier) : myKey(aKey), myModifier(GetValidModifier(aModifier))
{
}

bool Crimson::KeyBind::ContainsKey(eKey aKey) const
{
	return myKey == aKey || myModifier == aKey;
}

bool Crimson::KeyBind::ContainsKey(eKey aKey, bool& outIsModifierKey) const
{
	if (myKey == aKey)
	{
		outIsModifierKey = false;
		return true;
	}
	else if (myModifier == aKey)
	{
		outIsModifierKey = true;
		return true;
	}
	return false;
}

bool Crimson::KeyBind::HasModifier() const
{
	return myModifier != eKey::None;
}

bool Crimson::KeyBind::IsModifierKey(eKey aKey)
{
	switch (aKey)
	{
		case Crimson::eKey::None:
		case Crimson::eKey::Shift:
		case Crimson::eKey::Ctrl:
		case Crimson::eKey::Alt:
		case Crimson::eKey::LeftShift:
		case Crimson::eKey::RightShift:
		case Crimson::eKey::LeftCtrl:
		case Crimson::eKey::RightCtrl:
		case Crimson::eKey::LeftAlt:
		case Crimson::eKey::RightAlt:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

Crimson::eKey Crimson::KeyBind::GetValidModifier(eKey aKey)
{
	if (IsModifierKey(aKey))
	{
		return aKey;
	}
	return eKey::None;
}