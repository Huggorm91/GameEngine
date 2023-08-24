#pragma once
#include <Matrix4x4.hpp>

class Transform
{
public:
	Transform();
	Transform(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aRotation = {}, const CommonUtilities::Vector3f& aScale = {1.f, 1.f, 1.f});
	~Transform() = default;

	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	const CommonUtilities::Vector3f& GetPosition() const;

	void SetRotation(const CommonUtilities::Vector3f& aRotation);
	const CommonUtilities::Vector3f& GetRotation() const;

	void SetScale(const CommonUtilities::Vector3f& aScale);
	const CommonUtilities::Vector3f& GetScale() const;

	const CommonUtilities::Vector3f& GetWorldPosition() const;
	const CommonUtilities::Matrix4x4f& GetTransformMatrix() const;

	bool HasChanged() const;

private:
	bool myHasChanged;
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myRotation;
	CommonUtilities::Vector3f myScale;

	CommonUtilities::Vector3f myWorldPosition;
	CommonUtilities::Matrix4x4f myTransform;

	void UpdateTransform();
};