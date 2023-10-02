#pragma once
#include "../Component.h"
#include "GraphicsEngine/Camera/PerspectiveCamera.h"

class PerspectiveCameraComponent: public Component
{
public:
	PerspectiveCameraComponent();
	PerspectiveCameraComponent(const Json::Value& aJson, GameObject* aParent);
	PerspectiveCameraComponent(const PerspectiveCameraComponent& aLight) = default;
	PerspectiveCameraComponent(PerspectiveCameraComponent&& aLight) = default;
	~PerspectiveCameraComponent() = default;
	PerspectiveCameraComponent& operator=(const PerspectiveCameraComponent& aLight) = default;
	PerspectiveCameraComponent& operator=(PerspectiveCameraComponent&& aLight) = default;

	//Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const PerspectiveCameraComponent* GetTypePointer() const override;

private:
	PerspectiveCamera myCamera;
	CommonUtilities::Vector3f myOffsetPosition;
	CommonUtilities::Vector3f myOffsetRotation;
};