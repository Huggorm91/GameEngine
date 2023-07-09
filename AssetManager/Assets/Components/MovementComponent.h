#pragma once
#include "Component.h"
#include <Vector3.hpp>

class MovementComponent : public Component
{
public:
	MovementComponent() = default;
	~MovementComponent() = default;

	void Update() override;

	const MovementComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myMovementSpeed;
	CommonUtilities::Vector3f myRotationSpeed;

	CommonUtilities::Vector3f myVelocity;
	float myMaxVelocity;
};