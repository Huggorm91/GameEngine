#pragma once
#include "Container/MinHeap.hpp"
#include "UtilityFunctions.hpp"

namespace Crimson
{
	static const int MapWidth = 20;
	static const int MapHeight = 20;
	static const int TileCount = MapWidth * MapHeight;

	// The value of the Tile is used as weight, create new Tiles and change values as needed
	enum class Tile: unsigned int
	{
		Impassable, // Dont change Impassable!
		Passable
	};

	template <int MapHeight = Crimson::MapHeight, int MapWidth = Crimson::MapWidth>
	std::vector<int> AStar(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex, bool aUsingPreciseCalculation = false)
	{
		struct AStarNode
		{
			bool isVisited = false;
			int index = -1;
			int previous = -1;
			unsigned int distance = UINT_MAX;
			unsigned int heuristic = UINT_MAX;

			bool operator<(const AStarNode& aComparison)
			{
				return distance + heuristic < aComparison.distance + aComparison.heuristic;
			}
		};

		

		auto updateNeighbours = [&aMap, &aUsingPreciseCalculation, &anEndIndex]
		(const AStarNode& aNode, std::vector<AStarNode>& outMap, MinHeap<AStarNode>& outHeap)
		{
			auto calculateHeuristic = [&aUsingPreciseCalculation, &anEndIndex](const int anIndex)
			{
				unsigned int result = 0;

				const int row = anIndex / MapHeight;
				const int column = anIndex % MapWidth;

				const int endRow = anEndIndex / MapHeight;
				const int endColumn = anEndIndex % MapWidth;

				const int x = row - endRow;
				const int y = column - endColumn;

				if (aUsingPreciseCalculation)
				{
					result = static_cast<unsigned int>(sqrt((x * x) + (y * y)));
				}
				else
				{
					result = Abs(x) + Abs(y);
				}
				return result;
			};
			auto updateNode = [&aNode, &outHeap, &calculateHeuristic](unsigned int aDistance, AStarNode& outNode)
			{
				if (outNode.heuristic == UINT_MAX)
				{
					outNode.heuristic = calculateHeuristic(outNode.index);
				}
				if (outNode.distance > aDistance)
				{
					outNode.distance = aDistance;
					outNode.previous = aNode.index;
					outHeap.Enqueue(outNode);
				}
			};
			const int row = aNode.index / MapHeight;
			const int column = aNode.index % MapWidth;

			if (row > 0 && outMap[aNode.index - MapWidth].isVisited == false)
			{
				const int indexAbove = aNode.index - MapWidth;
				updateNode(aNode.distance + static_cast<int>(aMap[indexAbove]), outMap[indexAbove]);
			}
			if (row < MapHeight - 1 && outMap[aNode.index + MapWidth].isVisited == false)
			{
				const int indexBelow = aNode.index + MapWidth;
				updateNode(aNode.distance + static_cast<int>(aMap[indexBelow]), outMap[indexBelow]);
			}
			if (column > 0 && outMap[aNode.index - 1].isVisited == false)
			{
				const int indexLeft = aNode.index - 1;
				updateNode(aNode.distance + static_cast<int>(aMap[indexLeft]), outMap[indexLeft]);
			}
			if (column < MapHeight - 1 && outMap[aNode.index + 1].isVisited == false)
			{
				const int indexRight = aNode.index + 1;
				updateNode(aNode.distance + static_cast<int>(aMap[indexRight]), outMap[indexRight]);
			}
		};

		const int tileCount = MapWidth * MapHeight;
		assert(aMap.size() >= tileCount);

		std::vector<AStarNode> nodes;
		for (int index = 0; index < tileCount; index++)
		{
			nodes.emplace_back();
			nodes.back().index = index;
			if (aMap[index] == Tile::Impassable)
			{
				nodes.back().isVisited = true;
			}
		}
		nodes[aStartIndex].distance = 0;

		MinHeap<AStarNode> queue;
		queue.Enqueue(nodes[aStartIndex]);
		AStarNode* current = nullptr;

		while (queue.GetSize() > 0)
		{
			current = &nodes[queue.Dequeue().index];
			if (current->isVisited)
			{
				continue;
			}
			else if (current->index == anEndIndex)
			{
				break;
			}

			current->isVisited = true;
			updateNeighbours(*current, nodes, queue);
		}

		std::vector<int> result;
		if (current->index == anEndIndex)
		{
			while (current->previous != -1)
			{
				result.emplace_back(current->index);
				current = &nodes[current->previous];
			}
			result.emplace_back(current->index);
			std::reverse(result.begin(), result.end());
		}
		return result;
	}
}