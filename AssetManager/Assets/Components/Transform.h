#pragma once
#include <Matrix4x4.hpp>
#include <External/jsonCpp/json-forwards.h>

class Transform
{
public:
	Transform();
	Transform(const Json::Value& aJson);
	Transform(const CommonUtilities::Vector3f& aPosition, const CommonUtilities::Vector3f& aRotation = {}, const CommonUtilities::Vector3f& aScale = {1.f, 1.f, 1.f});
	Transform(const Transform& aTransform);
	Transform(Transform&& aTransform) noexcept;
	~Transform() = default;

	Transform& operator=(const Transform& aTransform);
	Transform& operator=(Transform&& aTransform) noexcept;

	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	const CommonUtilities::Vector3f& GetPosition() const;

	void SetRotation(const CommonUtilities::Vector3f& aRotation);
	const CommonUtilities::Vector3f& GetRotation() const;

	void SetScale(const CommonUtilities::Vector3f& aScale);
	const CommonUtilities::Vector3f& GetScale() const;

	const CommonUtilities::Vector4f& GetWorldPosition() const;
	const CommonUtilities::Matrix4x4f& GetTransformMatrix() const;

	void SetHasChanged(bool aState);
	bool HasChanged() const;

	void SetParent(Transform* aParent);
	void RemoveParent();

	void Update();

	void CreateImGuiComponents(const std::string& aWindowName);
	bool CreateMultipleSelectionImGuiComponents(const std::string& aWindowName);
	Json::Value ToJson() const;

private:
	bool myHasChanged;

	Transform* myParent;

	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myRotation;
	CommonUtilities::Vector3f myScale;

	CommonUtilities::Vector4f myWorldPosition;
	CommonUtilities::Matrix4x4f myTransform;

	void UpdateTransform();

	CommonUtilities::Matrix4x4f GetTransform() const;
	CommonUtilities::Matrix4x4f GetTotalTransform() const;
	CommonUtilities::Vector3f GetTotalPosition() const;
	CommonUtilities::Vector3f GetTotalRotation() const;
	CommonUtilities::Vector3f GetTotalScale() const;
};