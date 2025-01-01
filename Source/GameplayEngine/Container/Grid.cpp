#include "GameplayEngine.pch.h"
#include "Grid.h"
#ifndef NETWORK_SERVER
#include "GraphicsEngine/GraphicsEngine.h"

std::vector<LineHandle> localHandles; // Lazy solution since this is only rendered for an assignment
#endif // NETWORK_SERVER

Grid::Grid(const Crimson::Vector3f& aCenterPosition, float aCellSize, unsigned aWidth, unsigned aHeigth) :
	myCollider(),
	myCellSize(aCellSize),
	myWidth(aWidth),
	myHeigth(aHeigth)
{
	myCollider.InitWithPointAndSize(aCenterPosition, { aCellSize * aWidth, aCellSize * aHeigth });
#ifndef NETWORK_SERVER
	// Set up lines for rendering
	Crimson::Vector3f lowerLeft = { myCollider.GetMin().x, 10.f, myCollider.GetMin().y };
	Crimson::Vector3f UpperRight = { myCollider.GetMax().x, 10.f, myCollider.GetMax().y };
	Crimson::Vector3f upperLeft = { lowerLeft.x, 10.f, UpperRight.z };
	Crimson::Vector3f lowerRight = { UpperRight.x, 10.f, lowerLeft.z };

	// Border
	localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(lowerLeft, upperLeft, {1.f, 0.f, 0.f, 1.f}));
	localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(upperLeft, UpperRight, { 1.f, 0.f, 0.f, 1.f }));
	localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(UpperRight, lowerRight, { 1.f, 0.f, 0.f, 1.f }));
	localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(lowerRight, lowerLeft, { 1.f, 0.f, 0.f, 1.f }));

	// Vertical Lines
	Crimson::Vector3f upperPoint = upperLeft;
	Crimson::Vector3f lowerPoint = lowerLeft;
	for (unsigned i = 0; i < aWidth - 1; i++)
	{
		upperPoint.x += aCellSize;
		lowerPoint.x = upperPoint.x;

		localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(lowerPoint, upperPoint, { 1.f, 0.f, 0.f, 1.f }));
	}

	// Horizontal Lines
	Crimson::Vector3f leftPoint = lowerLeft;
	Crimson::Vector3f rightPoint = lowerRight;
	for (unsigned i = 0; i < aWidth - 1; i++)
	{
		leftPoint.z += aCellSize;
		rightPoint.z = leftPoint.z;

		localHandles.emplace_back(GraphicsEngine::Get().GetLineDrawer().AddLine(leftPoint, rightPoint, { 1.f, 0.f, 0.f, 1.f }));
	}
#endif // NETWORK_SERVER
}

Grid::~Grid()
{
#ifndef NETWORK_SERVER
	// GraphicsEngine seems to be deleted before GameEngine is so this causes a crash
	/*for (auto& handle : localHandles)
	{
		handle.Delete();
	}*/
	localHandles.clear();
#endif // NETWORK_SERVER
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
