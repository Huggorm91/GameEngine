#include "QuaternionTransform.h"

QuaternionTransform::QuaternionTransform(const Crimson::Matrix4x4f& aMatrix) :
	position(aMatrix.GetTranslation()),
	rotation(Crimson::QuatF(aMatrix).GetNormalized())
{}

QuaternionTransform::QuaternionTransform(const Crimson::Vector3f& aPosition, const Crimson::QuatF& aRotation) :
	position(aPosition),
	rotation(aRotation)
{}

void QuaternionTransform::Serialize(std::ostream& aStream) const
{
	position.Serialize(aStream);
	rotation.Serialize(aStream);
}

void QuaternionTransform::Deserialize(std::istream& aStream)
{
	position.Deserialize(aStream);
	rotation.Deserialize(aStream);
}

Crimson::Matrix4x4f QuaternionTransform::GetAsMatrix() const
{
	Crimson::Matrix4x4f result = rotation.GetAsMatrix3x3();
	result *= Crimson::Matrix4x4f::CreateTranslationMatrix(position);
	return result;
}

QuaternionTransform QuaternionTransform::Interpolate(const QuaternionTransform& aFrom, const QuaternionTransform& aTo, float aPercentage)
{
	return QuaternionTransform(Lerp(aFrom.position, aTo.position, aPercentage), Nlerp(aFrom.rotation, aTo.rotation, aPercentage));
}

void QuaternionTransform::Add(const QuaternionTransform& aTransform)
{
	position += aTransform.position;
	rotation *= aTransform.rotation;
}

void QuaternionTransform::Subtract(const QuaternionTransform& aTransform)
{
	position -= aTransform.position;
	rotation *= aTransform.rotation.GetInverse();
}