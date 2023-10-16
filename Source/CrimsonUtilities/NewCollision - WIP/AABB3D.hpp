#pragma once
#include "../AbstractClasses/CollisionObject.hpp"

namespace Crimson
{
	template <typename T>
	class AABB3D : public CollisionObject<T>
	{
	public:
		AABB3D();
		AABB3D(const AABB3D<T>& anAABB3D);
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);
		AABB3D(std::shared_ptr<Vector3<T>> aCenterPoint, const Vector3<T>& aSize, const Vector3<T>& anOffset = Vector3<T>());
		~AABB3D() = default;

		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		void InitWithPointAndSize(const Vector3<T>& aCenterPoint, const Vector3<T>& aSize);
		void InitWithPointAndSize(std::shared_ptr<Vector3<T>> aCenterPoint, const Vector3<T>& aSize, const Vector3<T>& anOffset = Vector3<T>());
		bool IsInside(const Vector3<T>& aPosition) const;

		void SetPosition(std::shared_ptr<Vector3<T>> aCenterPoint);
		void SetSize(const Vector3<T>& aSize);
		void SetOffset(const Vector3<T>& anOffset);

		Vector3<T> GetMin() const;
		Vector3<T> GetMax() const;
		Vector3<T> GetCenter() const;
		Vector3<T> GetOffset() const;
		Vector3<T> GetSize() const;

		bool TestCollision(const CollisionObject<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Sphere<T>& aSphere) const override { return IntersectionAABBSphere(*this, aSphere); }
		bool TestCollision(const AABB3D<T>& aBox) const override { return IntersectionAABBAABB(*this, aBox); }
		bool TestCollision(const Plane<T>& aPlane) const override { aPlane; return false; }
		bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const override { aPlaneVolume; return false; }
		bool TestCollision(const Ray<T>& aRay) const override { return IntersectionAABBRay(*this, aRay); }

	private:
		Vector3<T> mySize;
	};

	template<typename T>
	inline AABB3D<T>::AABB3D() : mySize(), CollisionObject<T>(eCollider::AABB)
	{
	}

	template<typename T>
	inline AABB3D<T>::AABB3D(const AABB3D<T>& anAABB3D) : mySize(anAABB3D.mySize), CollisionObject<T>(anAABB3D)
	{
	}

	template<typename T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax) : mySize((aMax - aMin) * 0.5), CollisionObject<T>(eCollider::AABB)
	{
		CollisionObject<T>::myPosition = std::make_shared<Vector3<T>>((aMin + aMax) * 0.5);
	}

	template<typename T>
	inline AABB3D<T>::AABB3D(std::shared_ptr<Vector3<T>> aCenterPoint, const Vector3<T>& aSize, const Vector3<T>& anOffset) :mySize(aSize * 0.5), CollisionObject<T>(eCollider::AABB, aCenterPoint, anOffset)
	{
	}

	template<typename T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myPosition = std::make_shared<Vector3<T>>((aMin + aMax) * 0.5);
		mySize = (aMax - aMin) * 0.5;
	}

	template<typename T>
	inline void AABB3D<T>::InitWithPointAndSize(const Vector3<T>& aCenterPoint, const Vector3<T>& aSize)
	{
		myPosition = std::make_shared<Vector3<T>>(aCenterPoint);
		mySize = aSize * 0.5;
	}

	template<typename T>
	inline void AABB3D<T>::InitWithPointAndSize(std::shared_ptr<Vector3<T>> aCenterPoint, const Vector3<T>& aSize, const Vector3<T>& anOffset)
	{
		myPosition = aCenterPoint;
		mySize = aSize * 0.5;
		myOffset = anOffset;
	}

	template<typename T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> min = GetMin();
		Vector3<T> max = GetMax();
		if (min.x <= aPosition.x && aPosition.x <= max.x &&
			min.y <= aPosition.y && aPosition.y <= max.y &&
			min.z <= aPosition.z && aPosition.z <= max.z)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline void AABB3D<T>::SetPosition(std::shared_ptr<Vector3<T>> aCenterPoint)
	{
		myPosition = aCenterPoint;
	}

	template<typename T>
	inline void AABB3D<T>::SetSize(const Vector3<T>& aSize)
	{
		mySize = aSize * 0.5;
	}

	template<typename T>
	inline void AABB3D<T>::SetOffset(const Vector3<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector3<T> AABB3D<T>::GetMin() const
	{
		return GetCenter() - mySize;;
	}

	template<typename T>
	inline Vector3<T> AABB3D<T>::GetMax() const
	{
		return GetCenter() + mySize;
	}

	template<typename T>
	inline Vector3<T> AABB3D<T>::GetCenter() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector3<T> AABB3D<T>::GetOffset() const
	{
		return myOffset;
	}

	template<typename T>
	inline Vector3<T> AABB3D<T>::GetSize() const
	{
		return mySize * 2;
	}
}