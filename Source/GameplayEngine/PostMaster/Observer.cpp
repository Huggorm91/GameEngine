#include "Observer.h"
#include "PostMaster.h"
#include "GameplayEngine/Engine.h"

Observer::~Observer()
{
	if (Engine::IsValid())
	{
		Engine::GetPostMaster().UnsubscribeFromAllMessages(this);
	}
}