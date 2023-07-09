#ifndef COLLISIONOBJECT2D
#define COLLISIONOBJECT2D
#include "../Enums/EnumCollisions.h"
#include "../Vector2.hpp"
#include <memory>
#include <bitset>

namespace CommonUtilities
{
	template <typename T>class AABB2D;
	template <typename T>class Circle;
	template <typename T>class Line;
	template <typename T>class LineVolume;
	template <typename T>class Ray2D;

	template <typename T>
	class CollisionObject2D
	{
	public:
		CollisionObject2D() : myPosition(std::make_shared<Vector2<T>>(Vector2<T>())), myLayersToCollideWith(), myLayer(), myType(), myOffset() {}
		CollisionObject2D(const CollisionObject2D& anObject) : myPosition(anObject.myPosition), myLayersToCollideWith(anObject.myLayersToCollideWith), myLayer(anObject.myLayer), myType(anObject.myType), myOffset(anObject.myOffset) {}
		virtual ~CollisionObject2D() = default;

		virtual	bool TestCollision(const CollisionObject2D<T>& anObject) const = 0;
		virtual	bool TestCollision(const Circle<T>& aCircle) const = 0;
		virtual	bool TestCollision(const AABB2D<T>& aBox) const = 0;
		virtual	bool TestCollision(const Line<T>& aLine) const = 0;
		virtual	bool TestCollision(const LineVolume<T>& aLineVolume) const = 0;
		virtual	bool TestCollision(const Ray2D<T>& aRay) const = 0;

		std::bitset<eCollisionLayer::eCount> GetLayersToCollideWith() const { return myLayersToCollideWith; }
		eCollisionLayer GetLayer() const { return myLayer; }

		void SetLayersToCollideWith(std::bitset<eCollisionLayer::eCount> someFlags) { myLayersToCollideWith = someFlags; }
		void SetLayer(eCollisionLayer aLayer) { myLayer = aLayer; }

	protected:
		Vector2<T> myOffset;
		std::shared_ptr<Vector2<T>> myPosition;
		std::bitset<eCollisionLayer::eCount> myLayersToCollideWith;
		eCollisionLayer myLayer;
		eCollider2D myType;

		CollisionObject2D(eCollider2D aType) : myPosition(std::make_shared<Vector2<T>>(Vector2<T>())), myLayersToCollideWith(), myLayer(), myType(aType), myOffset() {}
		CollisionObject2D(eCollider2D aType, std::shared_ptr<Vector2<T>> aPosition, const Vector2<T>& anOffset = Vector2<T>()) : myPosition(aPosition), myLayersToCollideWith(), myLayer(), myType(aType), myOffset(anOffset) {}
	};
}
#endif // !COLLISIONOBJECT2D

#ifndef INTERSECTION2DINCLUDE
#define INTERSECTION2DINCLUDE
#include "../Collision/Intersection2D.hpp"
#endif // !INTERSECTION2DINCLUDE
