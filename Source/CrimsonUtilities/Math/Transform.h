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

	void SetPosition(const Crimson::Vector3f& aPosition);
	const Crimson::Vector3f& GetPosition() const;

	void SetRotation(const Crimson::Vector3f& aRotation);
	const Crimson::Vector3f& GetRotation() const;

	void SetScale(const Crimson::Vector3f& aScale);
	const Crimson::Vector3f& GetScale() const;

	const Crimson::Vector4f& GetWorldPosition() const;
	const Crimson::Matrix4x4f& GetTransformMatrix() const;

	const Transform* GetParent() const;
	Transform* GetParent();
	void SetHasChanged(bool aState);
	bool HasChanged() const;

	void SetParent(Transform* aParent);
	void RemoveParent();

	void Update();

	Json::Value ToJson() const;
	void Serialize(std::ostream& aStream) const;
	void Deserialize(std::istream& aStream);

private:
	bool myHasChanged;

	Transform* myParent;

	Crimson::Vector3f myPosition;
	Crimson::Vector3f myRotation;
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