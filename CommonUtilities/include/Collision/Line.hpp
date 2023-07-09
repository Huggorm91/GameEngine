#pragma once
#include "../Vector2.hpp"

namespace CommonUtilities
{
	template <typename T>
	class Line
	{
	public:
		Line();
		Line(const Line<T>& aLine);
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		~Line() = default;

		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		bool IsInside(const Vector2<T>& aPosition) const;
		bool IsOnLine(const Vector2<T>& aPosition) const;

		Vector2<T> GetPoint() const;
		Vector2<T> GetDirection() const;
		Vector2<T> GetNormal() const;

	private:
		Vector2<T> myPoint;
		Vector2<T> myDirection;
	};

	template<typename T>
	inline Line<T>::Line() : myPoint(), myDirection()
	{
	}

	template<typename T>
	inline Line<T>::Line(const Line<T>& aLine) : myPoint(aLine.myPoint), myDirection(aLine.myDirection)
	{
	}

	template<typename T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1) : myPoint(aPoint0), myDirection(aPoint1 - aPoint0)
	{
		myDirection.Normalize();
	}

	template<typename T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint = aPoint0;
		myDirection = aPoint1 - aPoint0;
		myDirection.Normalize();
	}

	template<typename T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint = aPoint;
		myDirection = aDirection;
		myDirection.Normalize();
	}

	template<typename T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> aPositionDirection(aPosition - myPoint);
		if (GetNormal().Dot(aPositionDirection) <= T())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool Line<T>::IsOnLine(const Vector2<T>& aPosition) const
	{
		Vector2<T> aPositionDirection(aPosition - myPoint);
		if (GetNormal().Dot(aPositionDirection) == T())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetPoint() const
	{
		return myPoint;
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetDirection() const
	{
		return myDirection;
	}

	template<typename T>
	inline Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}
}
