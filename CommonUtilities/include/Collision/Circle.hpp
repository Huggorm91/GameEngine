#pragma once
#include "../Vector2.hpp"
#include <memory>

namespace CommonUtilities
{
	template <typename T>
	class Circle
	{
	public:
		Circle();
		Circle(const Circle<T>& aCircle);
		Circle(const Vector2<T>& aCenter, const T& aRadius);
		Circle(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset = Vector2<T>());
		~Circle() = default;

		void InitWithCenterAndRadius(const Vector2<T>& aCenter, const T& aRadius);
		void InitWithCenterAndRadius(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset = Vector2<T>());
		bool IsInside(const Vector2<T>& aPosition) const;

		void SetPosition(std::shared_ptr<Vector2<T>> aCenter);
		void SetOffset(const Vector2<T>& anOffset);
		void SetRadius(const T& aRadius);

		Vector2<T> GetCenter() const;
		Vector2<T> GetOffset() const;
		T GetRadius() const;

	private:
		Vector2<T> myOffset;
		std::shared_ptr<Vector2<T>> myPosition;
		T myRadius;
	};

	template<typename T>
	inline Circle<T>::Circle(): myPosition(std::make_shared<Vector2<T>>(Vector2<T>())), myRadius(), myOffset()
	{
	}

	template<typename T>
	inline Circle<T>::Circle(const Circle<T>& aCircle) : myPosition(aCircle.myPosition), myRadius(aCircle.myRadius), myOffset(aCircle.myOffset)
	{
	}

	template<typename T>
	inline Circle<T>::Circle(const Vector2<T>& aCenter, const T& aRadius): myPosition(std::make_shared<Vector2<T>>(aCenter)), myRadius(aRadius), myOffset()
	{
	}

	template<typename T>
	inline Circle<T>::Circle(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset) : myPosition(aCenter), myRadius(aRadius), myOffset(anOffset)
	{
	}

	template<typename T>
	inline void Circle<T>::InitWithCenterAndRadius(const Vector2<T>& aCenter, const T& aRadius)
	{
		myPosition = std::make_shared<Vector2<T>>(aCenter);
		myRadius = aRadius;
	}

	template<typename T>
	inline void Circle<T>::InitWithCenterAndRadius(std::shared_ptr<Vector2<T>> aCenter, const T& aRadius, const Vector2<T>& anOffset)
	{
		myPosition = aCenter;
		myRadius = aRadius;
		myOffset = anOffset;
	}

	template<typename T>
	inline bool Circle<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> distance(aPosition - GetCenter());
		if (distance.LengthSqr() <= (myRadius * myRadius))
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline void Circle<T>::SetPosition(std::shared_ptr<Vector2<T>> aCenter)
	{
		myPosition = aCenter;
	}

	template<typename T>
	inline void Circle<T>::SetOffset(const Vector2<T>& anOffset)
	{
		myOffset = anOffset;
	}

	template<typename T>
	inline void Circle<T>::SetRadius(const T& aRadius)
	{
		myRadius = aRadius;
	}

	template<typename T>
	inline Vector2<T> Circle<T>::GetCenter() const
	{
		return *myPosition + myOffset;
	}

	template<typename T>
	inline Vector2<T> Circle<T>::GetOffset() const
	{
		return myOffset;
	}

	template<typename T>
	inline T Circle<T>::GetRadius() const
	{
		return myRadius;
	}
}