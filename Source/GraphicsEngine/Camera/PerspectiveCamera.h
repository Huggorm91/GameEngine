#pragma once
#include "Input/InputObserver.h"
#include "../Drawer/LineHandle.h"

class LineHandle;

class PerspectiveCamera : public Crimson::InputObserver
{
public:
	PerspectiveCamera();
	~PerspectiveCamera() = default;

	void Init(const Crimson::Vector2f& aScreenSize, float aSpeed, float aRotationSpeed, float aMouseSensitivity);
	void Update();

	void ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey) override;
	void ReceiveEvent(Crimson::eInputAction anEvent, float aValue) override;

	const Crimson::Vector3f& GetPosition() const;
	void SetPosition(const Crimson::Vector3f& aPosition);
	void SetRotation(const Crimson::Vector3f& aRotation);

	void SetFOV(float aDegree);
	void SetPlanes(float aNearPlane, float aFarPlane);

	void SetMovementSpeed(float aSpeed);
	void SetRotationSpeed(float aSpeed);

	void SetMouseSensitivity(float aValue);

	Crimson::Vector3f TransformToClipSpace(const Crimson::Vector3f& aPoint) const;
	Crimson::Vector2f ConvertToScreenCoordinates(const Crimson::Vector3f& aClipSpacePosition, float& anOutScale) const;

private:
	Crimson::Matrix4x4f myCompassTransform;
	Crimson::Matrix4x4f myTransform;
	Crimson::Matrix4x4f myClipMatrix;

	Crimson::Vector3f myCompassOffset;
	Crimson::Vector3f myPosition;
	Crimson::Vector2f myRotation;

	Crimson::Vector2f myScreenResolution;

	float myRotationSpeed;
	float myMovementSpeed;
	float myMouseSensitivity;

	LineHandle myCompass;

	bool myIsAcceptingInput;
	bool myHasChanged;

	void UpdateTransform();
	float CalculateFOV(float aDegree) const;
};