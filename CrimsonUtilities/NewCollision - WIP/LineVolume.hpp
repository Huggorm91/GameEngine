#pragma once
#include "Line.hpp"
#include <vector>

namespace Crimson
{
	template <typename T>
	class LineVolume : public CollisionObject2D<T>
	{
	public:
		LineVolume();
		LineVolume(const std::vector<Line<T>>& aLineList);
		~LineVolume() = default;

		void AddLine(const Line<T>& aLine);
		bool IsInside(const Vector2<T>& aPosition) const;

		bool TestCollision(const CollisionObject2D<T>& anObject) const override { return anObject.TestCollision(*this); }
		bool TestCollision(const Circle<T>& aCircle) const override { aCircle; return false; }
		bool TestCollision(const AABB2D<T>& aBox) const override { aBox; return false; }
		bool TestCollision(const Line<T>& aLine) const override { aLine; return false; }
		bool TestCollision(const LineVolume<T>& aLineVolume) const override { aLineVolume; return false; }
		bool TestCollision(const Ray2D<T>& aRay) const override { aRay; return false; }

	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	inline LineVolume<T>::LineVolume() : myLines(), CollisionObject2D<T>(eCollider2D::LineVolume)
	{
	}

	template<typename T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList) : myLines(aLineList), CollisionObject2D<T>(eCollider2D::LineVolume)
	{
	}

	template<typename T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLines.push_back(aLine);
	}

	template<typename T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition) const
	{
		for (auto& line : myLines)
		{
			if (!line.IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}