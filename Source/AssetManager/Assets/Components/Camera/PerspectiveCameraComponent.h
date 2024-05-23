#pragma once
#include "../Component.h"
#include "CrimsonUtilities/Math/Matrix4x4.hpp"

BEGIN_COMPONENT(PerspectiveCameraComponent)
public:
	PerspectiveCameraComponent();
	PerspectiveCameraComponent(float aFoVDegree, float aNearPlane, float aFarPlane, bool anIsVerticalFoV = false);
	PerspectiveCameraComponent(const Json::Value& aJson);
	PerspectiveCameraComponent(const PerspectiveCameraComponent& aLight) = default;
	PerspectiveCameraComponent(PerspectiveCameraComponent&& aLight) = default;
	~PerspectiveCameraComponent() = default;
	PerspectiveCameraComponent& operator=(const PerspectiveCameraComponent& aLight) = default;
	PerspectiveCameraComponent& operator=(PerspectiveCameraComponent&& aLight) = default;

	void Update() override;

	void SetHorizontalFOV(float aDegree);
	void SetVerticalFOV(float aDegree);
	void SetPlanes(float aNearPlane, float aFarPlane);

	Crimson::Vector3f TransformToClipSpace(const Crimson::Vector3f& aPoint) const;
	Crimson::Vector2f ConvertToScreenCoordinates(const Crimson::Vector3f& aClipSpacePosition, float& anOutScale) const;

	void AddToPosition(const Crimson::Vector3f& aPosition);
	void SetPosition(const Crimson::Vector3f& aPosition);
	const Crimson::Vector3f& GetPosition() const;
	const Crimson::Vector3f& GetWorldPosition() const;

	void SetRotation(const Crimson::Vector3f& aDegreeRotation);
	void SetRadianRotation(const Crimson::Vector3f& aRadianRotation);
	Crimson::Vector3f GetRotation() const;
	const Crimson::Vector3f& GetRadianRotation() const;

	void TransformHasChanged() const override;

	Json::Value ToJson() const override;

	const Crimson::Matrix4x4f& GetView() const;
	const Crimson::Matrix4x4f& GetProjection() const;

private:
	Crimson::Matrix4x4f myView;
	Crimson::Matrix4x4f myProjection;
	Crimson::Vector3f myWorldPosition;

	Crimson::Vector3f myPosition;
	Crimson::Vector3f myRotation;

	float myNearPlane;
	float myFarPlane;
	float myFoVDegree;

	bool myIsVerticalFoV;
	bool myHasChanged;

	void UpdateTransform();
	float CalculateFOV(float aRadian) const;
};