#ifndef COLLISIONOBJECT3D
#define COLLISIONOBJECT3D
#include "../Enums/EnumCollisions.h"
#include "../Vector3.hpp"
#include <memory>
#include <bitset>

namespace Crimson
{
	template <typename T>class AABB3D;
	template <typename T>class Plane;
	template <typename T>class PlaneVolume;
	template <typename T>class Ray;
	template <typename T>class Sphere;

	template <typename T>
	class CollisionObject
	{
	public:
		CollisionObject(): myPosition(std::make_shared<Vector3<T>>(Vector3<T>())), myLayersToCollideWith(), myLayer(), myType(), myOffset() {}
		CollisionObject(const CollisionObject& anObject) : myPosition(anObject.myPosition), myLayersToCollideWith(anObject.myLayersToCollideWith), myLayer(anObject.myLayer), myType(anObject.myType), myOffset(anObject.myOffset) {}
		virtual ~CollisionObject() = default;

		virtual	bool TestCollision(const CollisionObject<T>& anObject) const = 0;
		virtual	bool TestCollision(const Sphere<T>& aSphere) const = 0;
		virtual	bool TestCollision(const AABB3D<T>& aBox) const = 0;
		virtual	bool TestCollision(const Plane<T>& aPlane) const = 0;
		virtual	bool TestCollision(const PlaneVolume<T>& aPlaneVolume) const = 0;
		virtual	bool TestCollision(const Ray<T>& aRay) const = 0;

		std::bitset<eCollisionLayer::Count> GetLayersToCollideWith() const { return myLayersToCollideWith; }
		eCollisionLayer GetLayer() const { return myLayer; }

		void SetLayersToCollideWith(std::bitset<eCollisionLayer::Count> someFlags) { myLayersToCollideWith = someFlags; }
		void SetLayer(eCollisionLayer aLayer) { myLayer = aLayer; }

	protected:
		Vector3<T> myOffset;
		std::shared_ptr<Vector3<T>> myPosition;
		std::bitset<eCollisionLayer::eCount> myLayersToCollideWith;
		eCollisionLayer myLayer;
		eCollider myType;

		CollisionObject(eCollider aType) : myPosition(std::make_shared<Vector3<T>>(Vector3<T>())), myLayersToCollideWith(), myLayer(), myType(aType), myOffset() {}
		CollisionObject(eCollider aType, std::shared_ptr<Vector3<T>> aPosition, const Vector3<T>& anOffset = Vector3<T>()) : myPosition(aPosition), myLayersToCollideWith(), myLayer(), myType(aType), myOffset(anOffset) {}
	};
}
#endif // !COLLISIONOBJECT3D

#ifndef INTERSECTION3DINCLUDE
#define INTERSECTION3DINCLUDE
#include "../Collision/Intersection.hpp"
#endif // !INTERSECTION3DINCLUDE