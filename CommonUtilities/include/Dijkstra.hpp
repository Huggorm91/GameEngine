#pragma once
#include "Container/MinHeap.hpp"

namespace CommonUtilities
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

	template <int MapHeight = CommonUtilities::MapHeight, int MapWidth = CommonUtilities::MapWidth>
	std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex)
	{
		struct DijkstraNode
		{
			bool isVisited = false;
			int index = -1;
			int previous = -1;
			unsigned int distance = UINT_MAX;

			bool operator<(const DijkstraNode& aComparison)
			{
				return distance < aComparison.distance;
			}
		};

		auto updateNeighbours = [&aMap](const DijkstraNode& aNode, std::vector<DijkstraNode>& outMap, MinHeap<DijkstraNode>& outHeap){
			auto updateNode = [&aNode, &outHeap](unsigned int aDistance, DijkstraNode& outNode){
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

		std::vector<DijkstraNode> nodes;
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

		MinHeap<DijkstraNode> queue;
		queue.Enqueue(nodes[aStartIndex]);
		DijkstraNode* current = nullptr;

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