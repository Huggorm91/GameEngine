#pragma once
#include "Component.h"
#include <Vector3.hpp>

class MovementComponent : public Component
{
public:
	MovementComponent() = default;
	~MovementComponent() = default;

	MovementComponent(const Json::Value& aJson, GameObject* aParent);

	void Update() override;

	//Json::Value ToJson() const override;
	const MovementComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myMovementSpeed;
	CommonUtilities::Vector3f myRotationSpeed;

	CommonUtilities::Vector3f myVelocity;
	float myMaxVelocity;
};