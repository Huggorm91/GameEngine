#pragma once
#include "../Vector2.hpp"
#include <memory>

namespace Crimson
{
	template <typename T>
	class Ray2D
	{
	public:
		Ray2D();
		Ray2D(const Ray2D<T>& aRay2D);
		Ray2D(const Vector2<T>& anOrigin, const Vector2<T>& aPoint);
		Ray2D(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint, const Vector2<T>& anOffset = Vector2<T>());
		~Ray2D() = default;

		void InitWith2Points(const Vector2<T>& anOrigin, const Vector2<T>& aPoint);
		void InitWith2Points(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint);
		void InitWithOriginAndDirection(const Vector2<T>& anOrigin, const Vector2<T>& aDirection);
		void InitWithOriginAndDirection(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aDirection, const Vector2<T>& anOffset = Vector2<T>());

		void SetDirection(const Vector2<T>& aDirection);
		void SetOrigin(std::shared_ptr<Vector2<T>> anOrigin);
		void SetOffset(const Vector2<T>& anOffset);

		// The direction is normalized
		Vector2<T> GetDirection() const;
		Vector2<T> GetOrigin() const;
		Vector2<T> GetOffset() const;

	private:
		std::shared_ptr<Vector2<T>> myOrigin;
		Vector2<T> myOffset;
		Vector2<T> myDirection;
	};

	template<typename T>
	inline Ray2D<T>::Ray2D(): myOrigin(std::make_shared<Vector2<T>>(Vector2<T>())), myDirection(), myOffset()
	{
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(const Ray2D<T>& aRay2D): myOrigin(aRay2D.myOrigin), myOffset(aRay2D.myOffset), myDirection(aRay2D.myDirection)
	{
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(const Vector2<T>& anOrigin, const Vector2<T>& aPoint): myOrigin(std::make_shared<Vector2<T>>(anOrigin)), myDirection(aPoint - anOrigin), myOffset()
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline Ray2D<T>::Ray2D(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint, const Vector2<T>& anOffset) : myOrigin(anOrigin), myDirection(aPoint - *anOrigin), myOffset(anOffset)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWith2Points(const Vector2<T>& anOrigin, const Vector2<T>& aPoint)
	{
		myOrigin = std::make_shared<Vector2<T>>(anOrigin);
		myDirection = aPoint - anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWith2Points(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aPoint)
	{
		myOrigin = anOrigin;
		myDirection = aPoint - *anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWithOriginAndDirection(const Vector2<T>& anOrigin, const Vector2<T>& aDirection)
	{
		myOrigin = std::make_shared<Vector2<T>>(anOrigin);
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::InitWithOriginAndDirection(std::shared_ptr<Vector2<T>> anOrigin, const Vector2<T>& aDirection, const Vector2<T>& anOffset)
	{
		myOrigin = anOrigin;
		myDirection = aDirection;
		myOffset = anOffset;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::SetDirection(const Vector2<T>& aDirection)
	{
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray2D<T>::SetOrigin(std::shared_ptr<Vector2<T>> anOrigin)
	{
		myOrigin = anOrigin;
	}

	template<typename T>
	inline void Ray2D<T>::SetOffset(const Vector2<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetDirection() const
	{
		return myDirection;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetOrigin() const
	{
		return *myOrigin + myOffset;
	}

	template<typename T>
	inline Vector2<T> Ray2D<T>::GetOffset() const
	{
		return myOffset;
	}
}