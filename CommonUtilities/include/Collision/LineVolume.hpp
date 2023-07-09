#pragma once
#include "Line.hpp"
#include <vector>

namespace CommonUtilities
{
	template <typename T>
	class LineVolume
	{
	public:
		LineVolume();
		LineVolume(const std::vector<Line<T>>& aLineList);
		~LineVolume() = default;

		void AddLine(const Line<T>& aLine);
		bool IsInside(const Vector2<T>& aPosition) const;

	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	inline LineVolume<T>::LineVolume(): myLines()
	{
	}

	template<typename T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList): myLines(aLineList)
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