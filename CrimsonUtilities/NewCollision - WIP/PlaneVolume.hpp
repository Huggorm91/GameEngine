#pragma once
#include "Plane.hpp"
#include <vector>

namespace Crimson
{
	template <typename T>
	class PlaneVolume : public CollisionObject<T>
	{
	public:
		PlaneVolume();
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);
		~PlaneVolume() = default;

		void AddPlane(const Plane<T>& aPlane);
		bool IsInside(const Vector3<T>& aPosition) const;

		bool TestCollision(const CollisionObject<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Sphere<T>& aSphere) const override { aSphere; return false; }
		bool TestCollision(const AABB3D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Plane<T>& aPlane) const override { aPlane; return false; }
		bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const override { aPlaneVolume; return false; }
		bool TestCollision(const Ray<T>& aRay) const override { aRay; return false; }

	private:
		std::vector<Plane<T>> myPlanes;
	};

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume() : myPlanes(), CollisionObject<T>(eCollider::PlaneVolume)
	{
	}

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList) : myPlanes(aPlaneList), CollisionObject<T>(eCollider::PlaneVolume)
	{
	}

	template<typename T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlanes.push_back(aPlane);
	}

	template<typename T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
	{
		for (auto& plane : myPlanes)
		{
			if (!plane.IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}