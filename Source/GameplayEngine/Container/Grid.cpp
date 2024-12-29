#include "GameplayEngine.pch.h"
#include "Grid.h"
#include <unordered_set>

Grid::Grid(const Crimson::Vector3f& aCenterPosition, float aCellSize, unsigned aWidth, unsigned aHeigth) :
	myCollider(),
	myCellSize(aCellSize),
	myWidth(aWidth),
	myHeigth(aHeigth)
{
	myCollider.InitWithPointAndSize(aCenterPosition, { aCellSize * aWidth, aCellSize * aHeigth });
}

int Grid::GetTileDistance(const Crimson::Vector3f& aFirstPosition, const Crimson::Vector3f& aSecondPosition) const
{
	auto firstGridIndex = GetGridIndex(aFirstPosition);
	if (firstGridIndex.x == -1)
	{
		return -1;
	}

	auto secondGridIndex = GetGridIndex(aSecondPosition);
	if (secondGridIndex.x == -1)
	{
		return -1;
	}

	auto difference = (firstGridIndex - secondGridIndex).Abs();
	return difference.x + difference.y;
}

Crimson::Vector2i Grid::GetGridIndex(const Crimson::Vector3f& aPosition) const
{
	if (!IsInside(aPosition))
	{
		return { -1, -1 };
	}

	Crimson::Vector2f positionInGrid = Crimson::Vector2f(aPosition.x, aPosition.z) - myCollider.GetMin();

	Crimson::Vector2i result = positionInGrid / myCellSize;
	result.x = Crimson::Clamp(result.x, 0, static_cast<int>(myWidth));
	result.y = Crimson::Clamp(result.y, 0, static_cast<int>(myHeigth));
	return result;
}

bool Grid::IsInside(const Crimson::Vector3f& aPosition) const
{
	return myCollider.IsInside({ aPosition.x, aPosition.z });
}
