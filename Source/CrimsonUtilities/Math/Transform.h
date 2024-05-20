#pragma once
#include "Matrix4x4.hpp"
namespace Json{ class Value; }

class Transform
{
public:
	Transform();
	Transform(const Json::Value& aJson);
	Transform(const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aRotation = {}, const Crimson::Vector3f& aScale = {1.f, 1.f, 1.f});
	Transform(const Transform& aTransform);
	Transform(Transform&& aTransform) noexcept;
	~Transform() = default;

	Transform& operator=(const Transform& aTransform);
	Transform& operator=(Transform&& aTransform) noexcept;

	Transform& operator+=(const Transform& aTransform);
	Transform& operator-=(const Transform& aTransform);
	Transform operator+(const Transform& aTransform) const;
	Transform operator-(const Transform& aTransform) const;

	void SetMatrix(const Crimson::Matrix4x4f& aMatrix);

	void SetPosition(const Crimson::Vector3f& aPosition);
	void AddToPosition(const Crimson::Vector3f& aChange);
	const Crimson::Vector3f& GetPosition() const;

	void SetRotationRadian(const Crimson::Vector3f& aRotation);
	void SetRotationDegree(const Crimson::Vector3f& aRotation);
	void AddToRotationRadian(const Crimson::Vector3f& aChange);
	void AddToRotationDegree(const Crimson::Vector3f& aChange);
	const Crimson::Vector3f& GetRotationRadian() const;
	Crimson::Vector3f GetRotationDegree() const;

	void SetScale(const Crimson::Vector3f& aScale);
	void AddToScale(const Crimson::Vector3f& aChange);
	const Crimson::Vector3f& GetScale() const;

	const Crimson::Vector4f& GetWorldPosition() const;
	const Crimson::Matrix4x4f& GetTransformMatrix() const;
	const Crimson::Matrix4x4f GetLocalTransformMatrix() const;

	const Transform* GetParent() const;
	Transform* GetParent();
	void SetHasChanged(bool aState);
	bool HasChanged() const;

	void SetParent(Transform* aParent);
	void RemoveParent();

	void Update(const bool aForceUpdate = false);

	void SetForwardVector(const Crimson::Vector3f& aDirection);
	Crimson::Vector3f GetForwardVector() const;
	Crimson::Vector3f GetRightVector() const;
	void SetUpVector(const Crimson::Vector3f& aDirection);
	Crimson::Vector3f GetUpVector() const;

	Json::Value ToJson() const;
	void Serialize(std::ostream& aStream) const;
	void Deserialize(std::istream& aStream);

private:
	bool myHasChangedInternal;
	bool myHasChangedThisFrame;

	Transform* myParent;

	Crimson::Vector3f myPosition;
	Crimson::Vector3f myRotation; // Stored as radians
	Crimson::Vector3f myScale;

	Crimson::Vector4f myWorldPosition;
	Crimson::Matrix4x4f myTransform;

	void UpdateTransform();

	Crimson::Matrix4x4f GetTransform() const;
	Crimson::Matrix4x4f GetTotalTransform() const;
	Crimson::Vector3f GetTotalPosition() const;
	Crimson::Vector3f GetTotalRotation() const;
	Crimson::Vector3f GetTotalScale() const;

	friend void CreateImGuiComponents(Transform& aTransform);
};