#pragma once
#include "../Component.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"

class PerspectiveCameraComponent: public Component
{
public:
	PerspectiveCameraComponent() = default;
	~PerspectiveCameraComponent() = default;

	const PerspectiveCameraComponent* GetTypePointer() const override;

private:
	PerspectiveCamera myCamera;
	CommonUtilities::Vector3f myOffsetPosition;
	CommonUtilities::Vector3f myOffsetRotation;
};