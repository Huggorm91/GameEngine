#pragma once
#include "2DColliders.hpp"

namespace Crimson
{
	template <typename T>
	bool IntersectionLineRay2D(const Line<T>& aLine, const Ray2D<T>& aRay2D, Vector2<T>& anOutIntersectionPoint);
	template <typename T>
	bool IntersectionLineFrontRay2D(const Line<T>& aLine, const Ray2D<T>& aRay2D, Vector2<T>& anOutIntersectionPoint);

	template <typename T>
	bool IntersectionAABB2DRay2D(const AABB2D<T>& anAABB, const Ray2D<T>& aRay2D);

	template <typename T>
	bool IntersectionAABB2DAABB2D(const AABB2D<T>& aFirstAABB, const AABB2D<T>& aSecondAABB);

	template <typename T>
	bool IntersectionCircleRay2D(const Circle<T>& aCircle, const Ray2D<T>& aRay2D);

	template <typename T>
	bool IntersectionCircleCircle(const Circle<T>& aFirstCircle, const Circle<T>& aSecondCircle);

	template <typename T>
	bool IntersectionAABB2DCircle(const AABB2D<T>& anAABBD, const Circle<T>& aCircle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	bool IntersectionLineRay2D(const Line<T>& aLine, const Ray2D<T>& aRay2D, Vector2<T>& anOutIntersectionPoint)
	{
		Vector2<T> lineNormal = aLine.GetNormal();
		Vector2<T> rayOrigin = aRay2D.GetOrigin();
		Vector2<T> rayDirection = aRay2D.GetDirection();

		T zero = T();
		T directionDotNormal = rayDirection.Dot(lineNormal);
		if (directionDotNormal == zero)
		{
			return aLine.IsOnLine(rayOrigin);
		}
		else
		{
			Vector2<T> linePoint = aLine.GetPoint();
			T tValue = rayDirection.Dot(linePoint - rayOrigin);
			if (tValue < zero)
			{
				return false;
			}

			anOutIntersectionPoint = rayOrigin + (rayDirection * tValue);
			return true;
		}
	}

	template<typename T>
	bool IntersectionLineFrontRay2D(const Line<T>& aLine, const Ray2D<T>& aRay2D, Vector2<T>& anOutIntersectionPoint)
	{
		Vector2<T> lineNormal = aLine.GetNormal();
		Vector2<T> rayOrigin = aRay2D.GetOrigin();
		Vector2<T> rayDirection = aRay2D.GetDirection();

		T zero = T();
		T directionDotNormal = rayDirection.Dot(lineNormal);
		if (directionDotNormal == zero)
		{
			return aLine.IsOnLine(rayOrigin);
		}
		else if (directionDotNormal < zero)
		{
			Vector2<T> linePoint = aLine.GetPoint();
			T tValue = rayDirection.Dot(linePoint - rayOrigin);
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
	bool IntersectionAABB2DRay2D(const AABB2D<T>& anAABB, const Ray2D<T>& aRay2D)
	{
		Vector2<T> rayOrigin = aRay2D.GetOrigin();
		Vector2<T> aabbMin = anAABB.GetMin();
		Vector2<T> aabbMax = anAABB.GetMax();
		bool isInside = true;

		if (rayOrigin.x < aabbMin.x)
		{
			isInside = false;
		}
		else if (aabbMax.x < rayOrigin.x)
		{
			isInside = false;
		}

		if (rayOrigin.y < aabbMin.y)
		{
			isInside = false;
		}
		else if (aabbMax.y < rayOrigin.y)
		{
			isInside = false;
		}

		if (isInside)
		{
			return true;
		}

		Vector2<T> rayDirection = aRay2D.GetDirection();
		Line<T> closestLineX;
		T zero = T();
		if (rayDirection.x < zero)
		{
			closestLineX.InitWith2Points(aabbMin, { aabbMin.x, aabbMax.y });
		}
		else
		{
			closestLineX.InitWith2Points(aabbMax, { aabbMax.x, aabbMin.y });
		}

		Vector2<T> intersectionPoint;
		if (IntersectionLineFrontRay2D(closestLineX, aRay2D, intersectionPoint))
		{
			if (aabbMin.x < intersectionPoint.x && intersectionPoint.x < aabbMax.x)
			{
				return true;
			}
		}

		Line<T> closestLineY;
		if (rayDirection.y < zero)
		{
			closestLineX.InitWith2Points(aabbMin, { aabbMax.x, aabbMin.y });
		}
		else
		{
			closestLineX.InitWith2Points(aabbMax, { aabbMin.x, aabbMax.y });
		}

		intersectionPoint.Zero();
		if (IntersectionLineFrontRay2D(closestLineY, aRay2D, intersectionPoint))
		{
			if (aabbMin.y < intersectionPoint.y && intersectionPoint.y < aabbMax.y)
			{
				return true;
			}
		}

		return false;
	}

	template <typename T>
	bool IntersectionAABB2DAABB2D(const AABB2D<T>& aFirstAABB, const AABB2D<T>& aSecondAABB)
	{
		Vector2<T> firstMax = aFirstAABB.GetMax();
		Vector2<T> firstMin = aFirstAABB.GetMin();

		Vector2<T> secondMax = aSecondAABB.GetMax();
		Vector2<T> secondMin = aSecondAABB.GetMin();

		return (firstMin.x <= secondMax.x && secondMin.x <= firstMax.x) &&
			(firstMin.y <= secondMax.y && secondMin.y <= firstMax.y);
	}

	template<typename T>
	bool IntersectionCircleRay2D(const Circle<T>& aCircle, const Ray2D<T>& aRay2D)
	{
		Vector2<T> rayOrigin = aRay2D.GetOrigin();
		Vector2<T> rayToCircleVector = aCircle.GetCenter() - rayOrigin;
		Vector2<T> rayDirection = aRay2D.GetDirection();
		T lengthToIntersection = rayDirection.Dot(rayToCircleVector);
		Vector2<T> intersectionPoint = rayOrigin + rayDirection * lengthToIntersection;

		return aCircle.IsInside(intersectionPoint);
	}

	template<typename T>
	bool IntersectionCircleCircle(const Circle<T>& aFirstCircle, const Circle<T>& aSecondCircle)
	{
		Vector2<T> centerToCenter = aFirstCircle.GetCenter() - aSecondCircle.GetCenter();
		T minimumDistance = aFirstCircle.GetRadius() + aSecondCircle.GetRadius();

		return centerToCenter.LengthSqr() <= minimumDistance * minimumDistance;
	}

	template<typename T>
	bool IntersectionAABB2DCircle(const AABB2D<T>& anAABBD, const Circle<T>& aCircle)
	{
		assert(anAABBD.GetMin() != anAABBD.GetMax());
		assert(aCircle.GetRadius() > T(0));

		T x = Crimson::Max<T>(anAABBD.GetMin().x, Crimson::Min<T>(aCircle.GetCenter().x, anAABBD.GetMax().x));
		T y = Crimson::Max<T>(anAABBD.GetMin().y, Crimson::Min<T>(aCircle.GetCenter().y, anAABBD.GetMax().y));

		return aCircle.IsInside(Vector2<T>(x, y));
	}
}