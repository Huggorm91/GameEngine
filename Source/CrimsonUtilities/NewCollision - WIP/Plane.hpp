#pragma once
#include "../AbstractClasses/CollisionObject.hpp"

namespace Crimson
{
	template <typename T>
	class Plane : public CollisionObject<T>
	{
	public:
		Plane();
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		~Plane() = default;

		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		bool IsInside(const Vector3<T>& aPosition) const;
		bool IsInPlane(const Vector3<T>& aPosition) const;
		Vector3<T> GetNormal() const;
		Vector3<T> GetPoint() const;

		bool TestCollision(const CollisionObject<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Sphere<T>& aSphere) const override { aSphere; return false; }
		bool TestCollision(const AABB3D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Plane<T>& aPlane) const override { aPlane; return false; }
		bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const override { aPlaneVolume; return false; }
		bool TestCollision(const Ray<T>& aRay) const override { aRay; return false; }

	private:
		Vector3<T> myPoint;
		Vector3<T> myNormal;
	};

	template<typename T>
	inline Plane<T>::Plane() : myPoint(), myNormal()
	{
	}

	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2) :
		myPoint(aPoint0), myNormal(Vector3<T>(aPoint1 - aPoint0).Cross(aPoint2 - aPoint0))
	{
		myNormal.Normalize();
	}

	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal) : myPoint(aPoint), myNormal(aNormal)
	{
		myNormal.Normalize();
	}

	template<typename T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		myNormal = Vector3<T>(aPoint1 - aPoint0).Cross(aPoint2 - aPoint0);
		myNormal.Normalize();
	}

	template<typename T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
		myNormal.Normalize();
	}

	template<typename T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> aPositionDirection(aPosition - myPoint);
		if (myNormal.Dot(aPositionDirection) <= T())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool Plane<T>::IsInPlane(const Vector3<T>& aPosition) const
	{
		Vector3<T> aPositionDirection(aPosition - myPoint);
		if (myNormal.Dot(aPositionDirection) == T())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline Vector3<T> Plane<T>::GetNormal() const
	{
		return myNormal;
	}

	template<typename T>
	inline Vector3<T> Plane<T>::GetPoint() const
	{
		return myPoint;
	}
}