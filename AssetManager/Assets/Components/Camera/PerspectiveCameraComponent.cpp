#include "AssetManager.pch.h"
#include "PerspectiveCameraComponent.h"

PerspectiveCameraComponent::PerspectiveCameraComponent(): Component(ComponentType::PerspectiveCamera), myCamera(), myOffsetPosition(), myOffsetRotation()
{
}

std::string PerspectiveCameraComponent::ToString() const
{
	return "PerspectiveCamera";
}

const PerspectiveCameraComponent* PerspectiveCameraComponent::GetTypePointer() const
{
	return this;
}
