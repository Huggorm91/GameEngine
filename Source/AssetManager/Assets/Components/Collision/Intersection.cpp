#include "AssetManager.pch.h"
#include "BoxColliderComponent.h"
#include "RayColliderComponent.h"
#include "SphereColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "Intersection.h"

bool Intersection(const BoxColliderComponent& aFirstBox, const BoxColliderComponent& aSecondBox)
{
    if (aFirstBox.IsValid() == false || aSecondBox.IsValid() == false)
    {
        return false;
    }

    return (
        aFirstBox.GetMin().x <= aSecondBox.GetMax().x &&
        aFirstBox.GetMax().x >= aSecondBox.GetMin().x &&

        aFirstBox.GetMin().y <= aSecondBox.GetMax().y &&
        aFirstBox.GetMax().y >= aSecondBox.GetMin().y &&

        aFirstBox.GetMin().z <= aSecondBox.GetMax().z &&
        aFirstBox.GetMax().z >= aSecondBox.GetMin().z
        );
}

bool Intersection(const BoxColliderComponent& aBox, const RayColliderComponent& aRay)
{
	if (aBox.IsValid() == false || aRay.IsValid() == false)
	{
		return false;
	}

	bool isInside = true;
	const float rayLength = aRay.GetLength();
	const Crimson::Vector3f& rayOrigin = aRay.GetOrigin();
	const Crimson::Vector3f& aabbCenter = aBox.GetCenter();
	const Crimson::Vector3f& aabbMin = aBox.GetMin();
	const Crimson::Vector3f& aabbMax = aBox.GetMax();
	const Crimson::Vector3f& halfSize = aBox.GetExtents();
	aabbCenter;
	halfSize;

	float xLength = 0.f;
	if (rayOrigin.x < aabbMin.x)
	{
		xLength = aabbMin.x - rayOrigin.x;
		if (rayLength < xLength)
		{
			return false;
		}
		isInside = false;
	}
	else if (aabbMax.x < rayOrigin.x)
	{
		xLength = aabbMax.x - rayOrigin.x;
		if (xLength < rayLength)
		{
			return false;
		}
		isInside = false;
	}
	else
	{
		xLength = -1.0f;
	}

	float yLength = 0.f;
	if (rayOrigin.y < aabbMin.y)
	{
		yLength = aabbMin.y - rayOrigin.y;
		if (rayLength < yLength)
		{
			return false;
		}
		isInside = false;
	}
	else if (aabbMax.y < rayOrigin.y)
	{
		yLength = aabbMax.y - rayOrigin.y;
		if (yLength < rayLength)
		{
			return false;
		}
		isInside = false;
	}
	else
	{
		yLength = -1.0f;
	}

	float zLength = 0.f;
	if (rayOrigin.z < aabbMin.z)
	{
		zLength = aabbMin.z - rayOrigin.z;
		if (rayLength < zLength)
		{
			return false;
		}
		isInside = false;
	}
	else if (aabbMax.z < rayOrigin.z)
	{
		zLength = aabbMax.z - rayOrigin.z;
		if (zLength < rayLength)
		{
			return false;
		}
		isInside = false;
	}
	else
	{
		zLength = -1.0f;
	}

	if (isInside)
	{
		return true;
	}

	int plane = 0;
	float longest = xLength;
	if (longest < yLength)
	{
		plane = 1;
		longest = yLength;
	}
	if (longest < zLength)
	{
		plane = 2;
		longest = zLength;
	}

	Crimson::Vector3f rayDelta = aRay.GetDirection() * rayLength;
	switch (plane)
	{
	case 0:
	{
		float y = rayOrigin.y + rayDelta.y * longest;
		if (y < aabbMin.y || aabbMax.y < y)
		{
			return false;
		}
		float z = rayOrigin.z + rayDelta.z * longest;
		if (z < aabbMin.z || aabbMax.z < z)
		{
			return false;
		}
		break;
	}
	case 1:
	{
		float x = rayOrigin.x + rayDelta.x * longest;
		if (x < aabbMin.x || aabbMax.x < x)
		{
			return false;
		}
		float z = rayOrigin.z + rayDelta.z * longest;
		if (z < aabbMin.z || aabbMax.z < z)
		{
			return false;
		}
		break;
	}
	case 2:
	{
		float x = rayOrigin.x + rayDelta.x * longest;
		if (x < aabbMin.x || aabbMax.x < x)
		{
			return false;
		}
		float y = rayOrigin.y + rayDelta.y * longest;
		if (y < aabbMin.y || aabbMax.y < y)
		{
			return false;
		}
		break;
	}
	}

	return true;
}

bool Intersection(const BoxColliderComponent& aBox, const SphereColliderComponent& aSphere)
{
	if (aBox.IsValid() == false || aSphere.IsValid() == false)
	{
		return false;
	}

	float x = Crimson::Max(aBox.GetMin().x, Crimson::Min(aSphere.GetCenter().x, aBox.GetMax().x));
	float y = Crimson::Max(aBox.GetMin().y, Crimson::Min(aSphere.GetCenter().y, aBox.GetMax().y));
	float z = Crimson::Max(aBox.GetMin().z, Crimson::Min(aSphere.GetCenter().z, aBox.GetMax().z));

	return  aSphere.IsInside(Crimson::Vector3f(x, y, z));
}

bool Intersection(const BoxColliderComponent& aBox, const CapsuleColliderComponent& aCapsule)
{
	if (aBox.IsValid() == false || aCapsule.IsValid() == false)
	{
		return false;
	}

    // TODO: Treat Capsule as Sphere if height == 0.f
    assert(!"Not Implemented");
    return false;
}

bool Intersection(const SphereColliderComponent& aFirstSphere, const SphereColliderComponent& aSecondSphere)
{
	if (aFirstSphere.IsValid() == false || aSecondSphere.IsValid() == false)
	{
		return false;
	}

	const Crimson::Vector3f& distance = aSecondSphere.GetCenter() - aFirstSphere.GetCenter();
	return distance.LengthSqr() <= aFirstSphere.GetRadiusSqr() + aSecondSphere.GetRadiusSqr();
}

bool Intersection(const SphereColliderComponent& aSphere, const RayColliderComponent& aRay)
{
	if (aSphere.IsValid() == false || aRay.IsValid() == false)
	{
		return false;
	}

	float lengthToIntersection = aRay.GetDirection().Dot(aSphere.GetCenter() - aRay.GetOrigin());
	if (aRay.GetLength() < lengthToIntersection)
	{
		return false;
	}

	Crimson::Vector3f intersectionPoint = aRay.GetOrigin() + (aRay.GetDirection() * lengthToIntersection);

	if (aSphere.IsInside(intersectionPoint))
	{
		return true;
	}
	return false;
}

bool Intersection(const SphereColliderComponent& aSphere, const CapsuleColliderComponent& aCapsule)
{
	if (aSphere.IsValid() == false || aCapsule.IsValid() == false)
	{
		return false;
	}

    assert(!"Not Implemented");
    return false;
}

bool Intersection(const CapsuleColliderComponent& aFirstCapsule, const CapsuleColliderComponent& aSecondCapsule)
{
	if (aFirstCapsule.IsValid() == false || aSecondCapsule.IsValid() == false)
	{
		return false;
	}

    // TODO: Treat Capsule as Sphere if height == 0.f
    assert(!"Not Implemented");
    return false;
}

bool Intersection(const CapsuleColliderComponent& aCapsule, const RayColliderComponent& aRay)
{
	if (aCapsule.IsValid() == false || aRay.IsValid() == false)
	{
		return false;
	}

    // TODO: Treat Capsule as Sphere if height == 0.f
    assert(!"Not Implemented");
    return false;
}

bool Intersection(const RayColliderComponent& aFirstRay, const RayColliderComponent& aSecondRay)
{
	aFirstRay;
	aSecondRay;
	// TODO: Find some way to check for intersection!
    return false;
}
