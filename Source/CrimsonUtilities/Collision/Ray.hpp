 #pragma once
#include "../Vector3.hpp"
#include <memory>

namespace Crimson
{
	template <typename T>
	class Ray
	{
	public:
		Ray();
		Ray(const Ray<T>& aRay);
		Ray(const Vector3<T>& anOrigin, const Vector3<T>& aPoint);
		Ray(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint, const Vector3<T>& anOffset = Vector3<T>());
		~Ray() = default;

		void InitWith2Points(const Vector3<T>& anOrigin, const Vector3<T>& aPoint);
		void InitWith2Points(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint);
		void InitWithOriginAndDirection(const Vector3<T>& anOrigin, const Vector3<T>& aDirection);
		void InitWithOriginAndDirection(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aDirection, const Vector3<T>& anOffset = Vector3<T>());

		void SetDirection(const Vector3<T>& aDirection);
		void SetOrigin(std::shared_ptr<Vector3<T>> anOrigin);
		void SetOffset(const Vector3<T>& anOffset);

		// The direction is normalized
		Vector3<T> GetDirection() const;
		Vector3<T> GetOrigin() const;
		Vector3<T> GetOffset() const;

	private:
		std::shared_ptr<Vector3<T>> myOrigin;
		Vector3<T> myOffset;
		Vector3<T> myDirection;
	};

	template<typename T>
	inline Ray<T>::Ray() : myOrigin(std::make_shared<Vector3<T>>(Vector3<T>())), myDirection(), myOffset()
	{
	}

	template<typename T>
	inline Ray<T>::Ray(const Ray<T>& aRay) : myOrigin(aRay.myOrigin), myOffset(aRay.myOffset), myDirection(aRay.myDirection)
	{
	}

	template<typename T>
	inline Ray<T>::Ray(const Vector3<T>& anOrigin, const Vector3<T>& aPoint): myOrigin(std::make_shared<Vector3<T>>(anOrigin)), myDirection(aPoint - anOrigin)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline Ray<T>::Ray(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint, const Vector3<T>& anOffset) : myOrigin(anOrigin), myDirection(aPoint - *anOrigin), myOffset(anOffset)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& anOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = std::make_shared<Vector3<T>>(anOrigin);
		myDirection = aPoint - anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWith2Points(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = anOrigin;
		myDirection = aPoint - *anOrigin;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& anOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = std::make_shared<Vector3<T>>(anOrigin);
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(std::shared_ptr<Vector3<T>> anOrigin, const Vector3<T>& aDirection, const Vector3<T>& anOffset)
	{
		myOrigin = anOrigin;
		myDirection = aDirection;
		myOffset = anOffset;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::SetDirection(const Vector3<T>& aDirection)
	{
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Ray<T>::SetOrigin(std::shared_ptr<Vector3<T>> anOrigin)
	{
		myOrigin = anOrigin;
	}

	template<typename T>
	inline void Ray<T>::SetOffset(const Vector3<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetDirection() const
	{
		return myDirection;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetOrigin() const
	{
		return *myOrigin + myOffset;
	}

	template<typename T>
	inline Vector3<T> Ray<T>::GetOffset() const
	{
		return myOffset;
	}
}