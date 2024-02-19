#pragma once
#include "../Component.h"
#include "Input/InputObserver.hpp"

class EditorCameraControllerComponent : public Component, public Crimson::InputObserver
{
public:
	EditorCameraControllerComponent();
	EditorCameraControllerComponent(float aSpeed, float aSensitivity);
	EditorCameraControllerComponent(const EditorCameraControllerComponent& aComponent) = default;
	EditorCameraControllerComponent(EditorCameraControllerComponent&& aComponent) noexcept = default;
	~EditorCameraControllerComponent();

	EditorCameraControllerComponent& operator=(const EditorCameraControllerComponent& aComponent) = default;
	EditorCameraControllerComponent& operator=(EditorCameraControllerComponent&& aComponent) noexcept = default;

	void Init(GameObject* aParent) override;

	inline std::string ToString() const override;
	inline const EditorCameraControllerComponent* GetTypePointer() const override;

	void ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey) override;

private:
	bool myIsMoving;
	float myMouseSensitivity;
	float mySpeed;
};