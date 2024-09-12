#pragma once
#include "../Component.h"
#include "GameplayEngine/Input/InputObserver.h"

BEGIN_COMPONENT_MULTI_INHERITANCE(FirstPersonCameraControllerComponent, Component, public InputObserver)
public:
	FirstPersonCameraControllerComponent();
	FirstPersonCameraControllerComponent(float aSpeed, float aSensitivity);
	FirstPersonCameraControllerComponent(const Json::Value& aJson);
	FirstPersonCameraControllerComponent(const FirstPersonCameraControllerComponent& aComponent) = default;
	FirstPersonCameraControllerComponent(FirstPersonCameraControllerComponent&& aComponent) noexcept = default;
	~FirstPersonCameraControllerComponent() = default;

	FirstPersonCameraControllerComponent& operator=(const FirstPersonCameraControllerComponent& aComponent) = default;
	FirstPersonCameraControllerComponent& operator=(FirstPersonCameraControllerComponent&& aComponent) noexcept = default;

	void Init(GameObject* aParent) override;

	void SetSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;
	Json::Value ToJson() const override;

private:
#ifndef _RETAIL
	bool myIsMoving;
#endif // !_RETAIL
	float myMouseSensitivity;
	float mySpeed;
};