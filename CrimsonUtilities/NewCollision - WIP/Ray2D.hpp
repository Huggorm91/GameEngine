#pragma once
#include "../AbstractClasses/CollisionObject2D.hpp"

namespace Crimson
{
	template <typename T>
	class Ray2D : public CollisionObject2D<T>
	{
	public:
		Ray2D();
		Ray2D(const Ray2D<T>& aRay2D);
		Ray2D(const Vector2<T>& anOrigin, const Vector2<T>& aPoint);
		Ray2D(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint, const Vector2<T>& anOffset = Vector2<T>());
		~Ray2D() = default;

		void InitWith2Points(const Vector2<T>& anOrigin, const Vector2<T>& aPoint);
		void InitWith2Points(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint);
		void InitWithOriginAndDirection(const Vector2<T>& anOrigin, const Vector2<T>& aDirection);
		void InitWithOriginAndDirection(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aDirection, const Vector2<T>& anOffset = Vector2<T>());

		void SetDirection(const Vector2<T>& aDirection);
		void SetOrigin(std::shared_ptr<Vector2<T>> anOrigin);
		void SetOffset(const Vector2<T>& anOffset);

		// The direction is normalized
		Vector2<T> GetDirection() const;
		Vector2<T> GetOrigin() const;
		Vector2<T> GetOffset() const;

		bool TestCollision(const CollisionObject2D<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Circle<T>& aCircle) const override { aCircle; return false; }
		bool TestCollision(const AABB2D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Line<T>& aLine) const override { aLine; return false; }
		bool TestCollision(const LineVolume<T>& aLineVolume) const override { aLineVolume; return false; }
		bool TestCollision(const Ray2D<T>& aRay) const override { aRay; return false; }

	private:
		Vector2<T> myDirection;
	};

	template<typename T>
	inline Ray2D<T>::Ray2D() : CollisionObject2D<T>(eCollider2D::Ray), myDirection()
	{
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(const Ray2D<T>& aRay2D) : CollisionObject2D<T>(aRay2D), myDirection(aRay2D.myDirection)
	{
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(const Vector2<T>& anOrigin, const Vector2<T>& aPoint) : CollisionObject2D<T>(eCollider2D::Ray, std::make_shared<Vector2<T>>(anOrigin)), myDirection(aPoint - anOrigin)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint, const Vector2<T>& anOffset) : CollisionObject2D<T>(eCollider2D::Ray, anOrigin, anOffset), myDirection(aPoint - *anOrigin)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWith2Points(const Vector2<T>& anOrigin, const Vector2<T>& aPoint)
	{
		myPosition = std::make_shared<Vector2<T>>(anOrigin);
		myDirection = aPoint - anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWith2Points(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint)
	{
		myPosition = anOrigin;
		myDirection = aPoint - *anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWithOriginAndDirection(const Vector2<T>& anOrigin, const Vector2<T>& aDirection)
	{
		myPosition = std::make_shared<Vector2<T>>(anOrigin);
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWithOriginAndDirection(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aDirection, const Vector2<T>& anOffset)
	{
		myPosition = anOrigin;
		myDirection = aDirection;
		myOffset = anOffset;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::SetDirection(const Vector2<T>& aDirection)
	{
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::SetOrigin(std::shared_ptr<Vector2<T>> anOrigin)
	{
		myPosition = anOrigin;
	}

	template<typename T>
	inline void Ray2D<T>::SetOffset(const Vector2<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetDirection() const
	{
		return myDirection;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetOrigin() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetOffset() const
	{
		return myOffset;
	}
}