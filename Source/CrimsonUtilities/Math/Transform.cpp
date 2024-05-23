#include "Transform.h"
#include "Conversions.hpp"
#include "CrimsonUtilities/Json/JsonVector.hpp"

Transform::Transform() :
	myScale(1.f, 1.f, 1.f),
	myHasChangedInternal(false),
	myHasChangedThisFrame(false),
	myParent(nullptr)
{}

Transform::Transform(const Json::Value& aJson) :
	myPosition(aJson["Position"]),
	myRotation(Crimson::DegreeToRadian(Crimson::Vector3f(aJson["Rotation"]))),
	myScale(aJson["Scale"]),
	myHasChangedInternal(true),
	myHasChangedThisFrame(false),
	myParent(nullptr)
{}

Transform::Transform(const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aRotation, const Crimson::Vector3f& aScale) :
	myPosition(aPosition),
	myRotation(aRotation),
	myScale(aScale),
	myHasChangedInternal(false),
	myHasChangedThisFrame(false),
	myParent(nullptr)
{
	UpdateTransform();
}

Transform::Transform(const Transform& aTransform) :
	myPosition(aTransform.myPosition),
	myRotation(aTransform.myRotation),
	myScale(aTransform.myScale),
	myHasChangedInternal(aTransform.myHasChangedInternal),
	myHasChangedThisFrame(aTransform.myHasChangedThisFrame),
	myTransform(aTransform.myTransform),
	myParent(nullptr),
	myWorldPosition(aTransform.myWorldPosition)
{}

Transform::Transform(Transform&& aTransform) noexcept :
	myPosition(aTransform.myPosition),
	myRotation(aTransform.myRotation),
	myScale(aTransform.myScale),
	myHasChangedInternal(aTransform.myHasChangedInternal),
	myHasChangedThisFrame(aTransform.myHasChangedThisFrame),
	myTransform(aTransform.myTransform),
	myParent(nullptr),
	myWorldPosition(aTransform.myWorldPosition)
{}

Transform& Transform::operator=(const Transform& aTransform)
{
	myPosition = aTransform.myPosition;
	myRotation = aTransform.myRotation;
	myScale = aTransform.myScale;
	myHasChangedInternal = aTransform.myHasChangedInternal;
	myTransform = aTransform.myTransform;
	myParent = nullptr;
	myWorldPosition = aTransform.myWorldPosition;
	return *this;
}

Transform& Transform::operator=(Transform&& aTransform) noexcept
{
	myPosition = aTransform.myPosition;
	myRotation = aTransform.myRotation;
	myScale = aTransform.myScale;
	myHasChangedInternal = aTransform.myHasChangedInternal;
	myTransform = aTransform.myTransform;
	myParent = nullptr;
	myWorldPosition = aTransform.myWorldPosition;
	return *this;
}

Transform& Transform::operator+=(const Transform& aTransform)
{
	myPosition += aTransform.myPosition;
	myRotation += aTransform.myRotation;
	myScale *= aTransform.myScale;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
	return *this;
}

Transform& Transform::operator-=(const Transform& aTransform)
{
	myPosition -= aTransform.myPosition;
	myRotation -= aTransform.myRotation;
	myScale /= aTransform.myScale;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
	return *this;
}

Transform Transform::operator+(const Transform& aTransform) const
{
	Transform result = *this;
	result += aTransform;
	return result;
}

Transform Transform::operator-(const Transform& aTransform) const
{
	Transform result = *this;
	result -= aTransform;
	return result;
}

void Transform::SetMatrix(const Crimson::Matrix4x4f& aMatrix)
{
	myTransform = aMatrix;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::AddToPosition(const Crimson::Vector3f& aChange)
{
	myPosition += aChange;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

const Crimson::Vector3f& Transform::GetPosition() const
{
	return myPosition;
}

void Transform::SetRotationRadian(const Crimson::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::SetRotationDegree(const Crimson::Vector3f& aRotation)
{
	myRotation = DegreeToRadian(aRotation);
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::AddToRotationRadian(const Crimson::Vector3f& aChange)
{
	myRotation += aChange;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::AddToRotationDegree(const Crimson::Vector3f& aChange)
{
	myRotation += DegreeToRadian(aChange);
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

const Crimson::Vector3f& Transform::GetRotationRadian() const
{
	return myRotation;
}

Crimson::Vector3f Transform::GetRotationDegree() const
{
	return RadianToDegree(myRotation);
}

void Transform::SetScale(const Crimson::Vector3f& aScale)
{
	myScale = aScale;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

void Transform::AddToScale(const Crimson::Vector3f& aChange)
{
	myScale += aChange;
	myHasChangedThisFrame = true;
	myHasChangedInternal = true;
}

const Crimson::Vector3f& Transform::GetScale() const
{
	return myScale;
}

const Crimson::Vector4f& Transform::GetWorldPosition() const
{
	if (myHasChangedInternal)
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
	return myWorldPosition;
}

const Crimson::Matrix4x4f& Transform::GetTransformMatrix() const
{
	if (myHasChangedInternal || (myParent && myParent->myHasChangedInternal))
	{
		const_cast<Transform*>(this)->UpdateTransform();
	}
	return myTransform;
}

const Crimson::Matrix4x4f Transform::GetLocalTransformMatrix() const
{
	return GetTransform();
}

const Transform* Transform::GetParent() const
{
	return myParent;
}

Transform* Transform::GetParent()
{
	return myParent;
}

void Transform::SetHasChanged(bool aState)
{
	myHasChangedInternal = aState;
	myHasChangedThisFrame = aState;
}

bool Transform::HasChanged() const
{
	return myHasChangedInternal || myHasChangedThisFrame;
}

void Transform::SetParent(Transform* aParent)
{
	myParent = aParent;
	UpdateTransform();
}

void Transform::RemoveParent()
{
	myParent = nullptr;
	UpdateTransform();
}

void Transform::Update(const bool aForceUpdate)
{
	myHasChangedThisFrame = false;

	if (myHasChangedInternal || (myParent && myParent->myHasChangedInternal) || aForceUpdate)
	{
		myHasChangedThisFrame = true;
		const_cast<Transform*>(this)->UpdateTransform();
	}
}

void Transform::UpdateTransform()
{
	myTransform = GetTotalTransform();
	myWorldPosition = myTransform.GetTranslation();
	myHasChangedInternal = false;
}

Crimson::Matrix4x4f Transform::GetTransform() const
{
	return Crimson::Matrix4x4f::CreateScaleMatrix(myScale) *
		Crimson::Matrix4x4f::CreateRotationMatrix(myRotation) *
		Crimson::Matrix4x4f::CreateTranslationMatrix(myPosition);
}

Crimson::Matrix4x4f Transform::GetTotalTransform() const
{
	if (myParent)
	{
		return GetTransform() * myParent->GetTotalTransform();
	}
	else
	{
		return GetTransform();
	}
}

Crimson::Vector3f Transform::GetTotalPosition() const
{
	if (myParent)
	{
		return myParent->GetTotalPosition() + myPosition;
	}
	else
	{
		return myPosition;
	}
}

Crimson::Vector3f Transform::GetTotalRotation() const
{
	if (myParent)
	{
		return myParent->GetTotalRotation() + myRotation;
	}
	else
	{
		return myRotation;
	}
}

Crimson::Vector3f Transform::GetTotalScale() const
{
	if (myParent)
	{
		return myParent->GetTotalScale() * myScale;
	}
	else
	{
		return myScale;
	}
}

void Transform::SetForwardVector(const Crimson::Vector3f& aDirection)
{
	Crimson::Vector3f forward = aDirection.GetNormalized();

	float angleY = atan2(forward.x, forward.z);
	float angleX = -asin(forward.y);

	SetRotationRadian({ angleX, angleY, 0 });
}

Crimson::Vector3f Transform::GetForwardVector() const
{
	Crimson::Vector4f forward4 = { 0, 0, 1, 0 };

	return Crimson::Vector3f(forward4 * myTransform);
}

Crimson::Vector3f Transform::GetRightVector() const
{
	Crimson::Vector4f right4 = { 1, 0, 0, 0 };

	return Crimson::Vector3f(right4 * myTransform);
}

void Transform::SetUpVector(const Crimson::Vector3f& aDirection)
{
	Crimson::Vector3f forward = aDirection.GetNormalized();

	float angleY = atan2(forward.x, forward.z);
	float angleX = -asin(forward.y) + 90.f;

	SetRotationRadian({ angleX, angleY, 0 });
}

Crimson::Vector3f Transform::GetUpVector() const
{
	Crimson::Vector4f up4 = { 0, 1, 0, 0 };

	return Crimson::Vector3f(up4 * myTransform);
}

Json::Value Transform::ToJson() const
{
	Json::Value result;
	result["Position"] = static_cast<Json::Value>(myPosition);
	result["Rotation"] = static_cast<Json::Value>(myRotation);
	result["Scale"] = static_cast<Json::Value>(myScale);
	return result;
}

void Transform::Serialize(std::ostream& aStream) const
{
	myPosition.Serialize(aStream);
	myRotation.Serialize(aStream);
	myScale.Serialize(aStream);
}

void Transform::Deserialize(std::istream& aStream)
{
	myPosition.Deserialize(aStream);
	myRotation.Deserialize(aStream);
	myScale.Deserialize(aStream);
	UpdateTransform();
}