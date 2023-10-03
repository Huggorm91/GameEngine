#pragma once
#include "3DColliders.hpp"
#include <unordered_map>

namespace Crimson
{
	template <typename T>
	class CollisionManager
	{
	public:
		CollisionManager() = default;
		~CollisionManager() = default;

		void CheckCollisions() const;

	private:
		std::unordered_map<eCollisionLayer, std::vector<CollisionObject<T>*>> myCollisionObjects;
		std::vector<std::pair<CollisionObject<T>*, CollisionObject<T>*>> myCollisions;
	};

	template<typename T>
	inline void CollisionManager<T>::CheckCollisions() const
	{
		for (auto& [layer, objectList] : myCollisionObjects)
		{
			for (auto& object : objectList)
			{
				// Get layersToCollideWith

				if (true /*Check if should collide with own layer*/)
				{
					// Nestled for loops where inner loop start at outer loop index+1
				}
				else
				{
					// Nestled rfor loops checking all objects in layer
				}
			}
		}
	}
}