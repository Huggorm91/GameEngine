#pragma once
#include "../Component.h"
#include "Math/Matrix4x4.hpp"

class PerspectiveCameraComponent : public Component
{
public:
	PerspectiveCameraComponent();
	PerspectiveCameraComponent(float aFoVDegree, float aNearPlane, float aFarPlane);
	PerspectiveCameraComponent(const Json::Value& aJson);
	PerspectiveCameraComponent(const PerspectiveCameraComponent& aCamera) = default;
	PerspectiveCameraComponent(PerspectiveCameraComponent&& aCamera) = default;
	~PerspectiveCameraComponent() = default;
	PerspectiveCameraComponent& operator=(const PerspectiveCameraComponent& aCamera) = default;
	PerspectiveCameraComponent& operator=(PerspectiveCameraComponent&& aCamera) = default;

	void Update() override;

	void SetHorizontalFOV(float aDegree);
	void SetPlanes(float aNearPlane, float aFarPlane);

	Crimson::Vector3f TransformToClipSpace(const Crimson::Vector3f& aPoint) const;
	Crimson::Vector2f ConvertToScreenCoordinates(const Crimson::Vector3f& aClipSpacePosition, float& anOutScale) const;

	void SetPosition(const Crimson::Vector3f& aPosition);
	void SetRotation(const Crimson::Vector3f& aDegreeRotation);
	const Crimson::Vector3f& GetPosition() const;
	const Crimson::Vector3f& GetWorldPosition() const;
	const Crimson::Vector3f& GetRotationRadian() const;
	Crimson::Vector3f GetRotationDegree() const;

	void TransformHasChanged() const override;

	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const PerspectiveCameraComponent* GetTypePointer() const override;

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

	bool myHasChanged;

	void UpdateTransform();
	float CalculateFOV(float aRadian) const;
};