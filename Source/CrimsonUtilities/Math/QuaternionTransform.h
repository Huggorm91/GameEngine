#pragma once
#include "Quaternion.hpp"

class QuaternionTransform
{
public:
	Crimson::Vector3f position;
	Crimson::QuatF rotation;

	QuaternionTransform() = default;
	QuaternionTransform(const Crimson::Matrix4x4f& aMatrix);
	QuaternionTransform(const Crimson::Vector3f& aPosition, const Crimson::QuatF& aRotation);

	void Serialize(std::ostream& aStream) const;
	void Deserialize(std::istream& aStream);

	Crimson::Matrix4x4f GetAsMatrix() const;
	static QuaternionTransform Interpolate(const QuaternionTransform& aFrom, const QuaternionTransform& aTo, float aPercentage);

	void Add(const QuaternionTransform& aTransform);
	void Subtract(const QuaternionTransform& aTransform);

	inline bool operator==(const QuaternionTransform& aTranform) const
	{
		return position == aTranform.position && rotation == aTranform.rotation;
	}
};