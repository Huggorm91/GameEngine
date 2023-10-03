#pragma once
#include "3DColliders.hpp"
#include "../UtilityFunctions.hpp"

namespace Crimson
{
	template <typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint);
	template <typename T>
	bool IntersectionPlaneFrontRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint);

	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& anAABB, const Ray<T>& aRay);

	template <typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	bool IntersectionAABBAABB(const AABB3D<T>& anAABB3D0, const AABB3D<T>& anAABB3D1);

	template <typename T>
	bool IntersectionAABBSphere(const AABB3D<T>& anAABB3D, const Sphere<T>& aSphere);

	template <typename T>
	bool IntersectionSphereSphere(const Sphere<T>& aSphere0, const Sphere<T>& aSphere1);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint)
	{
		Vector3<T> planeNormal = aPlane.GetNormal();				
		Vector3<T> rayOrigin = aRay.GetOrigin();
		Vector3<T> rayDirection = aRay.GetDirection();

		T zero = T();
		T directionDotNormal = rayDirection.Dot(planeNormal);
		if (directionDotNormal == zero)
		{
			if (aPlane.IsInPlane(rayOrigin))
			{
				return true;
			}
			return false;
		}
		else
		{
			Vector3<T> planePoint = aPlane.GetPoint();
			T dValue = planePoint.Dot(planeNormal);
			T tValue = (dValue - rayOrigin.Dot(planeNormal)) / directionDotNormal;
			if (tValue < zero)
			{
				return false;
			}

			anOutIntersectionPoint = rayOrigin + (rayDirection * tValue);
			return true;
		}
	}

	template<typename T>
	bool IntersectionPlaneFrontRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint)
	{
		Vector3<T> planeNormal = aPlane.GetNormal();			
		Vector3<T> rayOrigin = aRay.GetOrigin();
		Vector3<T> rayDirection = aRay.GetDirection();

		T zero = T();
		T directionDotNormal = rayDirection.Dot(planeNormal);
		if (directionDotNormal == zero)
		{
			if (aPlane.IsInPlane(rayOrigin))
			{
				return true;
			}
			return false;
		}
		else if (directionDotNormal < zero)
		{
			Vector3<T> planePoint = aPlane.GetPoint();
			T dValue = planePoint.Dot(planeNormal);
			T tValue = (dValue - rayOrigin.Dot(planeNormal)) / directionDotNormal;
			if (tValue < zero)
			{
				return false;
			}

			anOutIntersectionPoint = rayOrigin + (rayDirection * tValue);
			return true;
		}
		return false;
	}

	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& anAABB, const Ray<T>& aRay)
	{
		bool isInside = true;
		Vector3<T> rayOrigin = aRay.GetOrigin();
		Vector3<T> aabbCenter = anAABB.GetCenter();
		Vector3<T> aabbMin = anAABB.GetMin(); 
		Vector3<T> aabbMax = anAABB.GetMax();
		Vector3<T> originToCenter = aabbCenter - rayOrigin;
		Vector3<T> centerToMin = aabbMin - aabbCenter;
		Vector3<T> centerToMax = aabbMax - aabbCenter;

		T rayXLength = T();
		double xLength = 0.0;
		if (rayOrigin.x < aabbMin.x)
		{
			xLength = aabbMin.x - rayOrigin.x;
			rayXLength = originToCenter.x + centerToMax.x;
			if (rayXLength < xLength)
			{
				return false;
			}
			isInside = false;
		}
		else if (aabbMax.x < rayOrigin.x)
		{
			xLength = aabbMax.x - rayOrigin.x;
			rayXLength = originToCenter.x + centerToMin.x;
			if (xLength < rayXLength)
			{
				return false;
			}
			isInside = false;
		}
		else
		{
			xLength = -1.0f;
		}

		T rayYLength = T();
		double yLength = 0.0;
		if (rayOrigin.y < aabbMin.y)
		{
			yLength = aabbMin.y - rayOrigin.y;
			rayYLength = originToCenter.y + centerToMax.y;
			if (rayYLength < yLength)
			{
				return false;
			}
			isInside = false;
		}
		else if (aabbMax.y < rayOrigin.y)
		{
			yLength = aabbMax.y - rayOrigin.y;
			rayYLength = originToCenter.y + centerToMin.y;
			if (yLength < rayYLength)
			{
				return false;
			}
			isInside = false;
		}
		else
		{
			yLength = -1.0f;
		}

		T rayZLength = T();
		double zLength = 0.0;
		if (rayOrigin.z < aabbMin.z)
		{
			zLength = aabbMin.z - rayOrigin.z;
			rayZLength = originToCenter.z + centerToMax.z;
			if (rayZLength < zLength)
			{
				return false;
			}
			isInside = false;
		}
		else if (aabbMax.z < rayOrigin.z)
		{
			zLength = aabbMax.z - rayOrigin.z;
			rayZLength = originToCenter.z + centerToMin.z;
			if (zLength < rayZLength)
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

		Vector3<T> rayLength(rayXLength, rayYLength, rayZLength);
		Vector3<T> rayDelta = aRay.GetDirection() * rayLength;
		if (rayDelta == Vector3<T>())
		{
			return false;
		}

		int plane = 0;
		double longest = xLength;
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

		switch (plane)
		{
			case 0:
			{
				T y = static_cast<T>(rayOrigin.y + rayDelta.y * longest);
				if (y < aabbMin.y || aabbMax.y < y)
				{
					return false;
				}
				T z = static_cast<T>(rayOrigin.z + rayDelta.z * longest);
				if (z < aabbMin.z || aabbMax.z < z)
				{
					return false;
				}
				break;
			}
			case 1:
			{
				T x = static_cast<T>(rayOrigin.x + rayDelta.x * longest);
				if (x < aabbMin.x || aabbMax.x < x)
				{
					return false;
				}
				T z = static_cast<T>(rayOrigin.z + rayDelta.z * longest);
				if (z < aabbMin.z || aabbMax.z < z)
				{
					return false;
				}
				break;
			}
			case 2:
			{
				T x = static_cast<T>(rayOrigin.x + rayDelta.x * longest);
				if (x < aabbMin.x || aabbMax.x < x)
				{
					return false;
				}
				T y = static_cast<T>(rayOrigin.y + rayDelta.y * longest);
				if (y < aabbMin.y || aabbMax.y < y)
				{
					return false;
				}
				break;
			}
		}

		return true;
	}

	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> rayOrigin = aRay.GetOrigin();
		Vector3<T> rayToSphereVector = aSphere.GetCenter() - rayOrigin;
		Vector3<T> rayDirection = aRay.GetDirection();
		T lengthToIntersection = rayDirection.Dot(rayToSphereVector);
		Vector3<T> intersectionPoint = rayOrigin + (rayDirection * lengthToIntersection);

		if (aSphere.IsInside(intersectionPoint))
		{
			return true;
		}
		return false;
	}



	template <typename T>
	bool IntersectionAABBAABB(const AABB3D<T>& anAABB3D0, const AABB3D<T>& anAABB3D1)
	{
		assert(anAABB3D0.GetMin() != anAABB3D0.GetMax());
		assert(anAABB3D1.GetMin() != anAABB3D1.GetMax());

		return (
			anAABB3D0.GetMin().x <= anAABB3D1.GetMax().x &&
			anAABB3D0.GetMax().x >= anAABB3D1.GetMin().x &&

			anAABB3D0.GetMin().y <= anAABB3D1.GetMax().y &&
			anAABB3D0.GetMax().y >= anAABB3D1.GetMin().y &&

			anAABB3D0.GetMin().z <= anAABB3D1.GetMax().z &&
			anAABB3D0.GetMax().z >= anAABB3D1.GetMin().z
			);
	}

	template <typename T>
	bool IntersectionAABBSphere(const AABB3D<T>& anAABB3D, const Sphere<T>& aSphere)
	{
		assert(anAABB3D.GetMin() != anAABB3D.GetMax());
		assert(aSphere.GetRadius() > T(0));

		T x = Crimson::Max<T>(anAABB3D.GetMin().x, Crimson::Min<T>(aSphere.GetCenter().x, anAABB3D.GetMax().x));
		T y = Crimson::Max<T>(anAABB3D.GetMin().y, Crimson::Min<T>(aSphere.GetCenter().y, anAABB3D.GetMax().y));
		T z = Crimson::Max<T>(anAABB3D.GetMin().z, Crimson::Min<T>(aSphere.GetCenter().z, anAABB3D.GetMax().z));

		return  aSphere.IsInside(Vector3<T>(x, y, z));
	}

	template <typename T>
	bool IntersectionSphereSphere(const Sphere<T>& aSphere0, const Sphere<T>& aSphere1)
	{
		assert(aSphere0.GetRadius() > T(0));
		assert(aSphere1.GetRadius() > T(0));

		Vector3<T> dist = aSphere1.GetCenter() - aSphere0.GetCenter();

		return dist.Length() <= aSphere0.GetRadius() + aSphere1.GetRadius();
	}
}