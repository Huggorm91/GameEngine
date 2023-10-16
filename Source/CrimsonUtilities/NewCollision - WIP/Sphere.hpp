#pragma once
#include "../AbstractClasses/CollisionObject.hpp"

namespace Crimson
{
	template <typename T>
	class Sphere : public CollisionObject<T>
	{
	public:
		Sphere();
		Sphere(const Sphere<T>& aSphere);
		Sphere(const Vector3<T>& aCenter, const T& aRadius);
		Sphere(std::shared_ptr<Vector3<T>> aCenter, const T& aRadius, const Vector3<T>& anOffset = Vector3<T>());
		~Sphere() = default;

		void InitWithCenterAndRadius(const Vector3<T>& aCenter, const T& aRadius);
		void InitWithCenterAndRadius(std::shared_ptr<Vector3<T>> aCenter, const T& aRadius, const Vector3<T>& anOffset = Vector3<T>());
		bool IsInside(const Vector3<T>& aPosition) const;

		void SetPosition(std::shared_ptr<Vector3<T>> aCenter);
		void SetOffset(const Vector3<T>& anOffset);
		void SetRadius(const T& aRadius);

		Vector3<T> GetCenter() const;
		Vector3<T> GetOffset() const;
		T GetRadius() const;

		bool TestCollision(const CollisionObject<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Sphere<T>& aSphere) const override { return IntersectionSphereSphere(*this, aSphere); }
		bool TestCollision(const AABB3D<T>& aBox) const override { return IntersectionAABBSphere(aBox, *this); }
		bool TestCollision(const Plane<T>& aPlane) const override { aPlane; return false; }
		bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const override { aPlaneVolume; return false; }
		bool TestCollision(const Ray<T>& aRay) const override { return IntersectionSphereRay(*this, aRay); }

	private:
		T myRadius;
	};

	template<typename T>
	inline Sphere<T>::Sphere() : CollisionObject<T>(eCollider::Sphere), myRadius()
	{
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere) : CollisionObject<T>(aSphere), myRadius(aSphere.myRadius)
	{
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, const T& aRadius) : CollisionObject<T>(eCollider::Sphere, std::make_shared<Vector3<T>>(aCenter)), myRadius(aRadius)
	{
	}

	template<typename T>
	inline Sphere<T>::Sphere(std::shared_ptr<Vector3<T>> aCenter, const T& aRadius, const Vector3<T>& anOffset) : CollisionObject<T>(eCollider::Sphere, aCenter, anOffset), myRadius(aRadius)
	{
	}

	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, const T& aRadius)
	{
		myPosition = std::make_shared<Vector3<T>>(aCenter);
		myRadius = aRadius;
	}

	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(std::shared_ptr<Vector3<T>> aCenter, const T& aRadius, const Vector3<T>& anOffset)
	{
		myPosition = aCenter;
		myRadius = aRadius;
		myOffset = anOffset;
	}

	template<typename T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> distance(aPosition - GetCenter());
		if (distance.LengthSqr() <= (myRadius * myRadius))
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline void Sphere<T>::SetPosition(std::shared_ptr<Vector3<T>> aCenter)
	{
		myPosition = aCenter;
	}

	template<typename T>
	inline void Sphere<T>::SetOffset(const Vector3<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline void Sphere<T>::SetRadius(const T& aRadius)
	{
		myRadius = aRadius;
	}

	template<typename T>
	inline Vector3<T> Sphere<T>::GetCenter() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector3<T> Sphere<T>::GetOffset() const
	{
		return myOffset;
	}

	template<typename T>
	inline T Sphere<T>::GetRadius() const
	{
		return myRadius;
	}
}