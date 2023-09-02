#pragma once
#include <Matrix4x4.hpp>
#include <InputMapper.h>
#include "../Drawer/LineDrawer.h"

class PerspectiveCamera : public CommonUtilities::InputObserver
{
public:
	PerspectiveCamera();
	~PerspectiveCamera() = default;

	void Init(const CommonUtilities::Vector2f& aScreenSize, float aSpeed, float aRotationSpeed, float aMouseSensitivity);
	void Update();

	void ReceiveEvent(CommonUtilities::eInputEvent anEvent, CommonUtilities::eKey aKey) override;
	void ReceiveEvent(CommonUtilities::eInputAction anEvent, float aValue) override;

	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetRotation(const CommonUtilities::Vector3f& aRotation);

	void SetFOV(float aDegree);
	void SetPlanes(float aNearPlane, float aFarPlane);

	void SetMovementSpeed(float aSpeed);
	void SetRotationSpeed(float aSpeed);

	void SetMouseSensitivity(float aValue);

	CommonUtilities::Vector3f TransformToClipSpace(const CommonUtilities::Vector3f& aPoint) const;
	CommonUtilities::Vector2f ConvertToScreenCoordinates(const CommonUtilities::Vector3f& aClipSpacePosition, float& anOutScale) const;

private:
	CommonUtilities::Matrix4x4f myCompassTransform;
	CommonUtilities::Matrix4x4f myTransform;
	CommonUtilities::Matrix4x4f myClipMatrix;

	CommonUtilities::Vector3f myCompassOffset;
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector2f myRotation;

	CommonUtilities::Vector2f myScreenResolution;

	float myRotationSpeed;
	float myMovementSpeed;
	float myMouseSensitivity;

	LineDrawer::LineHandle myCompass;

	bool myIsAcceptingInput;
	bool myHasChanged;

	void UpdateTransform();
	float CalculateFOV(float aDegree) const;
};