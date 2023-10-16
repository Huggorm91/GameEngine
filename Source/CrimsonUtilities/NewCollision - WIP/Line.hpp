#pragma once
#include "../AbstractClasses/CollisionObject2D.hpp"

namespace Crimson
{
	template <typename T>
	class Line : public CollisionObject2D<T>
	{
	public:
		Line();
		Line(const Line<T>& aLine);
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		Line(std::shared_ptr<Vector2<T>> aPoint, const Vector2<T>& aDirection, const Vector2<T>& anOffset = Vector2<T>());
		~Line() = default;

		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);
		void InitWithPointAndDirection(std::shared_ptr<Vector2<T>> aPoint, const Vector2<T>& aDirection, const Vector2<T>& anOffset = Vector2<T>());

		bool IsInside(const Vector2<T>& aPosition) const;
		bool IsOnLine(const Vector2<T>& aPosition) const;

		Vector2<T> GetPoint() const;
		Vector2<T> GetDirection() const;
		Vector2<T> GetNormal() const;

		bool TestCollision(const CollisionObject2D<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Circle<T>& aCircle) const override { aCircle; return false; }
		bool TestCollision(const AABB2D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Line<T>& aLine) const override { aLine; return false; }
		bool TestCollision(const LineVolume<T>& aLineVolume) const override { aLineVolume; return false; }
		bool TestCollision(const Ray2D<T>& aRay) const override { aRay; return false; }

	private:
		Vector2<T> myNormal;

		Vector2<T> DirectionToNormal(const Vector2<T>& aDirection) const;
	};

	template<typename T>
	inline Line<T>::Line() : CollisionObject2D<T>(eCollider2D::Line), myNormal()
	{
	}

	template<typename T>
	inline Line<T>::Line(const Line<T>& aLine) : CollisionObject2D<T>(aLine), myNormal(aLine.myNormal)
	{
	}

	template<typename T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1) : CollisionObject2D<T>(eCollider2D::Line), myNormal(DirectionToNormal(aPoint1 - aPoint0))
	{
		myNormal.Normalize();
	}

	template<typename T>
	inline Line<T>::Line(std::shared_ptr<Vector2<T>> aPoint, const Vector2<T>& aDirection, const Vector2<T>& anOffset) : CollisionObject2D<T>(eCollider2D::Line, aPoint, anOffset), myNormal(DirectionToNormal(aDirection))
	{
	}

	template<typename T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		*myPosition = aPoint0;
		myNormal = DirectionToNormal(aPoint1 - aPoint0);
		myNormal.Normalize();
	}

	template<typename T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		*myPosition = aPoint;
		myNormal = DirectionToNormal(aDirection);
		myNormal.Normalize();
	}

	template<typename T>
	inline void Line<T>::InitWithPointAndDirection(std::shared_ptr<Vector2<T>> aPoint, const Vector2<T>& aDirection, const Vector2<T>& anOffset)
	{
		myPosition = aPoint;
		myOffset = anOffset;
		myNormal = DirectionToNormal(aDirection);
		myNormal.Normalize();
	}

	template<typename T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> aPositionDirection(aPosition - *myPosition);
		if (GetNormal().Dot(aPositionDirection) <= T())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool Line<T>::IsOnLine(const Vector2<T>& aPosition) const
	{
		Vector2<T> aPositionDirection(aPosition - *myPosition);
		if (GetNormal().Dot(aPositionDirection) == T() && (aPosition.x == *myPosition.x || aPosition.y == *myPosition.y))
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetPoint() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetDirection() const
	{
		return Vector2<T>(-myNormal.y, myNormal.x);
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetNormal() const
	{
		return myNormal;
	}

	template<typename T>
	inline Vector2<T> Line<T>::DirectionToNormal(const Vector2<T>& aDirection) const
	{
		return Vector2<T>(-aDirection.y, aDirection.x);
	}
}