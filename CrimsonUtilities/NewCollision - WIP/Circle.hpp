#pragma once
#include "../AbstractClasses/CollisionObject2D.hpp"

namespace Crimson
{
	template <typename T>
	class Circle : public CollisionObject2D<T>
	{
	public:
		Circle();
		Circle(const Circle<T>& aCircle);
		Circle(const Vector2<T>& aCenter, const T& aRadius);
		Circle(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset = Vector2<T>());
		~Circle() = default;

		void InitWithCenterAndRadius(const Vector2<T>& aCenter, const T& aRadius);
		void InitWithCenterAndRadius(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset = Vector2<T>());
		bool IsInside(const Vector2<T>& aPosition) const;

		void SetPosition(std::shared_ptr<Vector2<T>> aCenter);
		void SetOffset(const Vector2<T>& anOffset);
		void SetRadius(const T& aRadius);

		Vector2<T> GetCenter() const;
		Vector2<T> GetOffset() const;
		T GetRadius() const;

		bool TestCollision(const CollisionObject2D<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Circle<T>& aCircle) const override { aCircle; return false; }
		bool TestCollision(const AABB2D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Line<T>& aLine) const override { aLine; return false; }
		bool TestCollision(const LineVolume<T>& aLineVolume) const override { aLineVolume; return false; }
		bool TestCollision(const Ray2D<T>& aRay) const override { aRay; return false; }

	private:
		T myRadius;
	};

	template<typename T>
	inline Circle<T>::Circle() : CollisionObject2D<T>(eCollider2D::Circle), myRadius()
	{
	}

	template<typename T>
	inline Circle<T>::Circle(const Circle<T>& aCircle) : CollisionObject2D<T>(aCircle), myRadius(aCircle.myRadius)
	{
	}

	template<typename T>
	inline Circle<T>::Circle(const Vector2<T>& aCenter, const T& aRadius) : CollisionObject2D<T>(eCollider2D::Circle, std::make_shared<Vector2<T>>(aCenter)), myRadius(aRadius)
	{
	}

	template<typename T>
	inline Circle<T>::Circle(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset) : CollisionObject2D<T>(eCollider2D::Circle, aCenter, anOffset), myRadius(aRadius)
	{
	}

	template<typename T>
	inline void Circle<T>::InitWithCenterAndRadius(const Vector2<T>& aCenter, const T& aRadius)
	{
		CollisionObject2D<T>::myPosition = std::make_shared<Vector2<T>>(aCenter);
		myRadius = aRadius;
	}

	template<typename T>
	inline void Circle<T>::InitWithCenterAndRadius(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset)
	{
		CollisionObject2D<T>::myPosition = aCenter;
		CollisionObject2D<T>::myOffset = anOffset;
		myRadius = aRadius;
	}

	template<typename T>
	inline bool Circle<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> distance(aPosition - GetCenter());
		if (distance.LengthSqr() <= (myRadius * myRadius))
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline void Circle<T>::SetPosition(std::shared_ptr<Vector2<T>> aCenter)
	{
		CollisionObject2D<T>::myPosition = aCenter;
	}

	template<typename T>
	inline void Circle<T>::SetOffset(const Vector2<T>& anOffset)
	{
		CollisionObject2D<T>::myOffset = anOffset;
	}

	template<typename T>
	inline void Circle<T>::SetRadius(const T& aRadius)
	{
		myRadius = aRadius;
	}

	template<typename T>
	inline Vector2<T> Circle<T>::GetCenter() const
	{
		return *CollisionObject2D<T>::myPosition + CollisionObject2D<T>::myOffset;
	}

	template<typename T>
	inline Vector2<T> Circle<T>::GetOffset() const
	{
		return CollisionObject2D<T>::myOffset;
	}

	template<typename T>
	inline T Circle<T>::GetRadius() const
	{
		return myRadius;
	}
}