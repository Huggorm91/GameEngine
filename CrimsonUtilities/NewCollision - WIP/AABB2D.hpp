#pragma once
#include "../AbstractClasses/CollisionObject2D.hpp"

namespace Crimson
{
	template <typename T>
	class AABB2D : public CollisionObject2D<T>
	{
	public:
		AABB2D();
		AABB2D(const AABB2D<T>& anAABB2D);
		AABB2D(const Vector2<T>& aMin, const Vector2<T>& aMax);
		AABB2D(std::shared_ptr<Vector2<T>> aCenterPoint, const Vector2<T>& aSize, const Vector2<T>& anOffset = Vector2<T>());
		~AABB2D() = default;

		void InitWithMinAndMax(const Vector2<T>& aMin, const Vector2<T>& aMax);
		void InitWithPointAndSize(const Vector2<T>& aCenterPoint, const Vector2<T>& aSize);
		void InitWithPointAndSize(std::shared_ptr<Vector2<T>> aCenterPoint, const Vector2<T>& aSize, const Vector2<T>& anOffset = Vector2<T>());
		bool IsInside(const Vector2<T>& aPosition) const;

		void SetPosition(std::shared_ptr<Vector2<T>> aCenterPoint);
		void SetSize(const Vector2<T>& aSize);
		void SetOffset(const Vector2<T>& anOffset);

		Vector2<T> GetMin() const;
		Vector2<T> GetMax() const;
		Vector2<T> GetCenter() const;
		Vector2<T> GetOffset() const;
		Vector2<T> GetSize() const;

		bool TestCollision(const CollisionObject2D<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Circle<T>& aCircle) const override { return IntersectionAABB2DCircle(*this, aCircle); }
		bool TestCollision(const AABB2D<T>& aBox) const override { return IntersectionAABB2DAABB2D(*this, aBox); }
		bool TestCollision(const Line<T>& aLine) const override { aLine; return false; }
		bool TestCollision(const LineVolume<T>& aLineVolume) const override { aLineVolume; return false; }
		bool TestCollision(const Ray2D<T>& aRay) const override { return IntersectionAABB2DRay2D(*this, aRay); }

	private:
		Vector2<T> mySize;
	};

	template<typename T>
	inline AABB2D<T>::AABB2D() : CollisionObject2D<T>(eCollider2D::AABB), mySize()
	{
	}

	template<typename T>
	inline AABB2D<T>::AABB2D(const AABB2D<T>& anAABB2D) : mySize(anAABB2D.mySize), CollisionObject2D<T>(anAABB2D)
	{
	}

	template<typename T>
	inline AABB2D<T>::AABB2D(const Vector2<T>& aMin, const Vector2<T>& aMax) : mySize((aMax - aMin) * 0.5), CollisionObject2D<T>(anAABB2D, std::make_shared<Vector2<T>>((aMin + aMax) * 0.5))
	{
	}

	template<typename T>
	inline AABB2D<T>::AABB2D(std::shared_ptr<Vector2<T>> aCenterPoint, const Vector2<T>& aSize, const Vector2<T>& anOffset) : mySize(aSize * 0.5), CollisionObject2D<T>(anAABB2D, aCenterPoint, anOffset)
	{
	}

	template<typename T>
	inline void AABB2D<T>::InitWithMinAndMax(const Vector2<T>& aMin, const Vector2<T>& aMax)
	{
		myPosition = std::make_shared<Vector2<T>>((aMin + aMax) * 0.5);
		mySize = (aMax - aMin) * 0.5;
	}

	template<typename T>
	inline void AABB2D<T>::InitWithPointAndSize(const Vector2<T>& aCenterPoint, const Vector2<T>& aSize)
	{
		myPosition = std::make_shared<Vector2<T>>(aCenterPoint);
		mySize = aSize * 0.5;
	}

	template<typename T>
	inline void AABB2D<T>::InitWithPointAndSize(std::shared_ptr<Vector2<T>> aCenterPoint, const Vector2<T>& aSize, const Vector2<T>& anOffset)
	{
		myPosition = aCenterPoint;
		mySize = aSize * 0.5;
		myOffset = anOffset;
	}

	template<typename T>
	inline bool AABB2D<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> min = GetMin();
		Vector2<T> max = GetMax();
		if (min.x <= aPosition.x && aPosition.x <= max.x &&
			min.y <= aPosition.y && aPosition.y <= max.y)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline void AABB2D<T>::SetPosition(std::shared_ptr<Vector2<T>> aCenterPoint)
	{
		myPosition = aCenterPoint;
	}

	template<typename T>
	inline void AABB2D<T>::SetSize(const Vector2<T>& aSize)
	{
		mySize = aSize * 0.5;
	}

	template<typename T>
	inline void AABB2D<T>::SetOffset(const Vector2<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector2<T> AABB2D<T>::GetMin() const
	{
		return GetCenter() - mySize;
	}
	template<typename T>
	inline Vector2<T> AABB2D<T>::GetMax() const
	{
		return  GetCenter() + mySize;
	}
	template<typename T>
	inline Vector2<T> AABB2D<T>::GetCenter() const
	{
		return *myPosition + myOffset;
	}
	template<typename T>
	inline Vector2<T> AABB2D<T>::GetOffset() const
	{
		return myOffset;
	}
	template<typename T>
	inline Vector2<T> AABB2D<T>::GetSize() const
	{
		return mySize * 2;
	}
}