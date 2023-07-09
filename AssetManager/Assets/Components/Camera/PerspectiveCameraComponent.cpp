#include "AssetManager.pch.h"
#include "PerspectiveCameraComponent.h"

const PerspectiveCameraComponent* PerspectiveCameraComponent::GetTypePointer() const
{
	return this;
}
