#pragma once
#include "Plane.hpp"
#include <vector>

namespace Crimson
{
	template <typename T>
	class PlaneVolume
	{
	public:
		PlaneVolume();
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);
		~PlaneVolume() = default;

		void AddPlane(const Plane<T>&aPlane);
		bool IsInside(const Vector3<T>& aPosition) const;

	private:
		std::vector<Plane<T>> myPlanes;
	};

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(): myPlanes()
	{
	}

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList): myPlanes(aPlaneList)
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