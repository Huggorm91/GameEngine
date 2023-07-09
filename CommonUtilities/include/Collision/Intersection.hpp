#pragma once
#include "AABB3D.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "PlaneVolume.hpp"
#include "../UtilityFunctions.hpp"

namespace CommonUtilities
{
	template <typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint);
	template <typename T>
	bool IntersectionPlaneFrontRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint);

	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& anAABB, const Ray<T>& aRay);
	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& anAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint);

	template <typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool IntersectionAABBAABB(const AABB3D<T>& anAABB3D0, const AABB3D<T>& anAABB3D1);

	template <typename T>
	bool AABBIntersect(const AABB3D<T>& aBoxA, const AABB3D<T>& aBoxB, CommonUtilities::Vector3<float>& aOutCollisionNormal, float& aOutIntersectionDepth);

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
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		{
#define LEFT	0
#define MIDDLE	1
#define RIGHT	2

			aOutIntersectionPoint = {};
			Vector3<T>  minPoint = aAABB.GetMin();
			Vector3<T>  maxPoint = aAABB.GetMax();
			Vector3<T> rayOrigin = aRay.GetOrigin();
			Vector3<T> rayDirection = aRay.GetDirection();

			bool inside = true;
			char quadrant[3];
			Vector3<T> candidatePlane;

			//i = x,y,z for less code
			//decides if in the area between min and max
			for (int i = 0; i < 3; i++)
			{
				if (rayOrigin[i] < minPoint[i])
				{
					quadrant[i] = LEFT;
					candidatePlane[i] = minPoint[i];
					inside = false;
				}
				else if (rayOrigin[i] > maxPoint[i])
				{
					quadrant[i] = RIGHT;
					candidatePlane[i] = maxPoint[i];
					inside = false;
				}
				else
				{
					quadrant[i] = MIDDLE;
				}
			}

			// all Quadrants are intersected
			if (inside)
			{
				aOutIntersectionPoint = rayOrigin;
				return true;
			}


			Vector3<T> tAtIntersectionOfPlane;
			for (int i = 0; i < 3; i++)
			{
				//If i am not in the volume that extrudes from the normal of the face1. and my direction toward that origin is not 0 
				if (quadrant[i] != MIDDLE && rayDirection[i] != 0.)
				{
					tAtIntersectionOfPlane[i] = (candidatePlane[i] - rayOrigin[i]) / rayDirection[i];
				}
				else
				{
					tAtIntersectionOfPlane[i] = -1.;
				}
			}

			//The largest t value is the intersected plane. Check what axis it corresponds to.
			int whichPlane = 0;
			for (int i = 1; i < 3; i++)
			{
				if (tAtIntersectionOfPlane[whichPlane] < tAtIntersectionOfPlane[i])
				{
					whichPlane = i;
				}
			}

			if (tAtIntersectionOfPlane[whichPlane] < 0)
			{
				return  false;
			}

			Vector3<T> hitPoint = Vector3<T>();
			for (int i = 0; i < 3; i++)
			{
				if (whichPlane != i)
				{
					//Set the coordinates in the plane of intersection
					hitPoint[i] = rayOrigin[i] + tAtIntersectionOfPlane[whichPlane] * rayDirection[i];
					if (hitPoint[i] < minPoint[i] || hitPoint[i] > maxPoint[i])
					{
						return false;
					}
				}
				else
				{
					hitPoint[i] = candidatePlane[i];
				}
			}
			aOutIntersectionPoint = hitPoint;
			return true;
		}
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
	bool AABBIntersect(const AABB3D<T>& aBoxA, const AABB3D<T>& aBoxB,
					   CommonUtilities::Vector3<float>& aOutCollisionNormal, float& aOutIntersectionDepth)
	{
		CommonUtilities::Vector3<float> faces[6] =
		{
			 CommonUtilities::Vector3<float>(-1, 0,  0),
			 CommonUtilities::Vector3<float>(1,  0,  0),
			 CommonUtilities::Vector3<float>(0,  -1, 0),
			 CommonUtilities::Vector3<float>(0,  1,  0),
			 CommonUtilities::Vector3<float>(0,  0,  -1),
			 CommonUtilities::Vector3<float>(0,  0,  1),
		};

		float distances[6] =
		{
			(aBoxB.GetMax().x - aBoxA.GetMin().x),
			(aBoxA.GetMax().x - aBoxB.GetMin().x),
			(aBoxB.GetMax().y - aBoxA.GetMin().y),
			(aBoxA.GetMax().y - aBoxB.GetMin().y),
			(aBoxB.GetMax().z - aBoxA.GetMin().z),
			(aBoxA.GetMax().z - aBoxB.GetMin().z),
		};

		for (int i = 0; i < 6; i++)
		{
			if (distances[i] < -0.001f) return false;

			if ((i == 0) || (distances[i] < aOutIntersectionDepth))
			{
				aOutCollisionNormal = faces[i];
				aOutIntersectionDepth = distances[i];
			}
		}

		if (aOutIntersectionDepth <= 0.001f)
		{
			aOutIntersectionDepth = 0.f;
		}

		return true;
	}

	template <typename T>
	bool IntersectionAABBSphere(const AABB3D<T>& anAABB3D, const Sphere<T>& aSphere)
	{
		assert(anAABB3D.GetMin() != anAABB3D.GetMax());
		assert(aSphere.GetRadius() > T(0));

		T x = CommonUtilities::Max<T>(anAABB3D.GetMin().x, CommonUtilities::Min<T>(aSphere.GetCenter().x, anAABB3D.GetMax().x));
		T y = CommonUtilities::Max<T>(anAABB3D.GetMin().y, CommonUtilities::Min<T>(aSphere.GetCenter().y, anAABB3D.GetMax().y));
		T z = CommonUtilities::Max<T>(anAABB3D.GetMin().z, CommonUtilities::Min<T>(aSphere.GetCenter().z, anAABB3D.GetMax().z));

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