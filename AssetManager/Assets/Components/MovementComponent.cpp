#include "AssetManager.pch.h"
#include "MovementComponent.h"

void MovementComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

}

const MovementComponent* MovementComponent::GetTypePointer() const
{
	return this;
}
