#include "Observer.h"
#include "PostMaster.h"
#include "../Engine/Engine.h"

Crimson::Observer::~Observer()
{
	if (Engine::IsValid())
	{
		Engine::GetPostMaster().UnsubscribeFromAllMessages(this);
	}
}
