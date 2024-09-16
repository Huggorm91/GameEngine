#include "InputObserver.h"
#include "InputMapper.h"
#include "GameplayEngine/Engine.h"

InputObserver::InputObserver(): myPointer(this)
{
}

InputObserver::InputObserver(const InputObserver&) : InputObserver()
{
}

InputObserver::InputObserver(InputObserver&& anObserver) noexcept: InputObserver()
{
	anObserver.DetachAll();
}

InputObserver::~InputObserver()
{
	DetachAll();
}

void InputObserver::Attach(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (Engine::IsValid() && !Engine::IsServer())
	{
		Engine::GetInputMapper().Attach(myPointer, anEvent, aKey);
	}
}

void InputObserver::Attach(Crimson::eInputAction anAction, Crimson::eKeyAction aKeyAction)
{
	if (Engine::IsValid() && !Engine::IsServer())
	{
		Engine::GetInputMapper().Attach(myPointer, anAction, aKeyAction);
	}
}

void InputObserver::Detach(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (Engine::IsValid() && !Engine::IsServer())
	{
		Engine::GetInputMapper().Detach(myPointer, anEvent, aKey);
	}
}

void InputObserver::Detach(Crimson::eInputAction anAction, Crimson::eKeyAction aKeyAction)
{
	if (Engine::IsValid() && !Engine::IsServer())
	{
		Engine::GetInputMapper().Detach(myPointer, anAction, aKeyAction);
	}
}

void InputObserver::DetachAll()
{
	if (Engine::IsValid() && !Engine::IsServer())
	{
		Engine::GetInputMapper().DetachAll(myPointer);
	}
}

void InputObserver::RenewInputObserverPointer()
{
	DetachAll();
	myPointer = this;
}
