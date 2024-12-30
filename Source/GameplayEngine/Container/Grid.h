#pragma once
#include "CrimsonUtilities/Collision/AABB2D.hpp"

class Grid
{
public:
	Grid(const Crimson::Vector3f& aCenterPosition, float aCellSize, unsigned aWidth, unsigned aHeigth);
	~Grid();

	// Returns -1 if any of the objects are outside the grid.
	// Example (Distance from X, X would be 0):
	// | 2 | 3 | 4 |
	// | 1 | 2 | 3 |
	// | X | 1 | 2 |
	int GetTileDistance(const Crimson::Vector3f& aFirstPosition, const Crimson::Vector3f& aSecondPosition) const;

private:

	Crimson::AABB2D<float> myCollider;
	float myCellSize;
	unsigned myWidth;
	unsigned myHeigth;

	Crimson::Vector2i GetGridIndex(const Crimson::Vector3f& aPosition) const;

	bool IsInside(const Crimson::Vector3f& aPosition) const;
};