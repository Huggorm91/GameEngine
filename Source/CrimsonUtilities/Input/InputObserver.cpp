#include "InputObserver.h"
#include "InputMapper.h"
#include "../Engine/Engine.h"

Crimson::InputObserver::~InputObserver()
{
	if (Engine::IsValid())
	{
		Engine::GetInputMapper().DetachAll(this);
	}
}
