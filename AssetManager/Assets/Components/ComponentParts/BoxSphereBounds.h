#pragma once
#include <Vector3.hpp>
#include <TgaFbxStructs.h>

class BoxSphereBounds
{
public:
	BoxSphereBounds();
	BoxSphereBounds(const TGA::FBX::BoxSphereBounds& aBounds);
	BoxSphereBounds(const TGA::FBX::Box& aBounds);

	void Init(const CommonUtilities::Vector3f& aCenter, const CommonUtilities::Vector3f& aSize);
	void Init(const CommonUtilities::Vector3f& aCenter, float aRadius);

	CommonUtilities::Vector3f GetMin() const;
	CommonUtilities::Vector3f GetMax() const;
	CommonUtilities::Vector3f GetBoxSize() const;
	const CommonUtilities::Vector3f& GetBoxExtents() const;
	const CommonUtilities::Vector3f& GetCenter() const;
	float GetRadius() const;
	bool IsValid() const;

private:
	CommonUtilities::Vector3f myBoxExtents;
	CommonUtilities::Vector3f myCenter;
	float myRadius;
	bool myIsValid;
};