#include "Observer.h"
#include "PostMaster.h"
#include "GameplayEngine/Engine.h"

// TODO: Implement the same safety of moving the observer as InputObserver has

Observer::~Observer()
{
	if (Engine::IsValid())
	{
		Engine::GetPostMaster().UnsubscribeFromAllMessages(this);
	}
}