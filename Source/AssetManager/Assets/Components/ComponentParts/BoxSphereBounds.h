#pragma once
#include "CrimsonUtilities/Math/Vector3.hpp"
#include "ThirdParty/FBXImporter/include/TgaFbxStructs.h"

class BoxSphereBounds
{
public:
	BoxSphereBounds();
	BoxSphereBounds(const TGA::FBX::BoxSphereBounds& aBounds);
	BoxSphereBounds(const TGA::FBX::Box& aBounds);

	void Init(const Crimson::Vector3f& aCenter, const Crimson::Vector3f& aSize);
	void Init(const Crimson::Vector3f& aCenter, float aRadius);

	Crimson::Vector3f GetMin() const;
	Crimson::Vector3f GetMax() const;
	Crimson::Vector3f GetBoxSize() const;
	const Crimson::Vector3f& GetBoxExtents() const;
	const Crimson::Vector3f& GetCenter() const;
	float GetRadius() const;
	bool IsValid() const;

private:
	Crimson::Vector3f myBoxExtents;
	Crimson::Vector3f myCenter;
	float myRadius;
	bool myIsValid;
};