#pragma once
#include "../Component.h"
#include "../ComponentParts/Skeleton.h"

class SkeletonComponent : public Component
{
public:
	SkeletonComponent();
	SkeletonComponent(Skeleton& aSkeleton);
	~SkeletonComponent() = default;

	const std::string& GetName() const;

	bool HasBone(const std::string& aName) const;
	const Bone& GetBone(unsigned int anIndex) const;
	const Bone& GetBone(const std::string& aName) const;

	const std::vector<Bone>& GetBones() const;

	const SkeletonComponent* GetTypePointer() const override;
private:
	Skeleton* mySkeleton;
};