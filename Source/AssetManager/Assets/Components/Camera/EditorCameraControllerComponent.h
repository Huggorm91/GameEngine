#pragma once
#include "../Component.h"
#include "GameplayEngine/Input/InputObserver.h"

BEGIN_COMPONENT_MULTI_INHERITANCE(EditorCameraControllerComponent, Component, public InputObserver)
public:
	EditorCameraControllerComponent();
	EditorCameraControllerComponent(float aSpeed, float aSensitivity);
	EditorCameraControllerComponent(const EditorCameraControllerComponent& aComponent) = default;
	EditorCameraControllerComponent(EditorCameraControllerComponent&& aComponent) noexcept = default;
	~EditorCameraControllerComponent() = default;

	EditorCameraControllerComponent& operator=(const EditorCameraControllerComponent& aComponent) = default;
	EditorCameraControllerComponent& operator=(EditorCameraControllerComponent&& aComponent) noexcept = default;

	void Init(GameObject* aParent) override;

	void SetSpeed(float aSpeed);
	void SetMouseSensitivity(float aSensitivity);

	void ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey) override;

private:
	bool myIsMoving;
	float myMouseSensitivity;
	float mySpeed;
};