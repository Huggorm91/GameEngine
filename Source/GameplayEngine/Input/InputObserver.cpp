#include "InputObserver.h"
#include "InputMapper.h"
#include "GameplayEngine/Engine.h"

InputObserver::~InputObserver()
{
	if (Engine::IsValid())
	{
		Engine::GetInputMapper().DetachAll(this);
	}
}
