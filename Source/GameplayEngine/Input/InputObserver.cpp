#include "InputObserver.h"
#include "InputMapper.h"
#include "GameplayEngine/Engine.h"

Crimson::InputObserver::~InputObserver()
{
	if (Engine::IsValid())
	{
		Engine::GetInputMapper().DetachAll(this);
	}
}
