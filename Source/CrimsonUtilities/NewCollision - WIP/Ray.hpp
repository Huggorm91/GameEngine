#pragma once
#include "../AbstractClasses/CollisionObject.hpp"

namespace Crimson
{
	template <typename T>
	class Ray : public CollisionObject<T>
	{
	public:
		Ray();
		Ray(const Ray<T>& aRay);
		Ray(const Vector3<T>& anOrigin, const Vector3<T>& aPoint);
		Ray(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint, const Vector3<T>& anOffset = Vector3<T>());
		~Ray() = default;

		void InitWith2Points(const Vector3<T>& anOrigin, const Vector3<T>& aPoint);
		void InitWith2Points(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint);
		void InitWithOriginAndDirection(const Vector3<T>& anOrigin, const Vector3<T>& aDirection);
		void InitWithOriginAndDirection(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aDirection, const Vector3<T>& anOffset = Vector3<T>());

		void SetDirection(const Vector3<T>& aDirection);
		void SetOrigin(std::shared_ptr<Vector3<T>> anOrigin);
		void SetOffset(const Vector3<T>& anOffset);

		// The direction is normalized
		Vector3<T> GetDirection() const;
		Vector3<T> GetOrigin() const;
		Vector3<T> GetOffset() const;

		bool TestCollision(const CollisionObject<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Sphere<T>& aSphere) const override { aSphere; return false; }
		bool TestCollision(const AABB3D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Plane<T>& aPlane) const override { aPlane; return false; }
		bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const override { aPlaneVolume; return false; }
		bool TestCollision(const Ray<T>& aRay) const override { aRay; return false; }

	private:
		Vector3<T> myDirection;
	};

	template<typename T>
	inline Ray<T>::Ray() : CollisionObject<T>(eCollider::Ray), myDirection()
	{
	}

	template<typename T>
	inline Ray<T>::Ray(const Ray<T>& aRay) : CollisionObject<T>(aRay), myDirection(aRay.myDirection)
	{
	}

	template<typename T>
	inline Ray<T>::Ray(const Vector3<T>& anOrigin, const Vector3<T>& aPoint) : CollisionObject<T>(eCollider::Ray, std::make_shared<Vector3<T>>(anOrigin)), myDirection(aPoint - anOrigin)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline Ray<T>::Ray(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint, const Vector3<T>& anOffset) : CollisionObject<T>(eCollider::Ray, anOrigin, anOffset), myDirection(aPoint - *anOrigin)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& anOrigin, const Vector3<T>& aPoint)
	{
		myPosition = std::make_shared<Vector3<T>>(anOrigin);
		myDirection = aPoint - anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWith2Points(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint)
	{
		myPosition = anOrigin;
		myDirection = aPoint - *anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& anOrigin, const Vector3<T>& aDirection)
	{
		myPosition = std::make_shared<Vector3<T>>(anOrigin);
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aDirection, const Vector3<T>& anOffset)
	{
		myPosition = anOrigin;
		myDirection = aDirection;
		myOffset = anOffset;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::SetDirection(const Vector3<T>& aDirection)
	{
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::SetOrigin(std::shared_ptr<Vector3<T>> anOrigin)
	{
		myPosition = anOrigin;
	}

	template<typename T>
	inline void Ray<T>::SetOffset(const Vector3<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetDirection() const
	{
		return myDirection;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetOrigin() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetOffset() const
	{
		return myOffset;
	}
}