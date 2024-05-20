#pragma once
#include <vector>
#include <functional>
#include "Math.hpp"

namespace Crimson
{
	template <class T>
	void SelectionSort(std::vector<T>& aVector);

	template <class T>
	void BubbleSort(std::vector<T>& aVector);

	template <class T>
	void QuickSort(std::vector<T>& aVector);
	template <class T, class U>
	void QuickSort(std::vector<T>& aVector, std::function<bool(U, U)> aCompareFunction);

	template <class T>
	void MergeSort(std::vector<T>& aVector);

	template <class T>
	void HeapSort(std::vector<T>& aVector);

	template <class T>
	void ShellSort(std::vector<T>& aVector);

	template<class T>
	void InsertionSort(std::vector<T>& aVector);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<class T>
	void SelectionSort(std::vector<T>& aVector)
	{
		for (int outerIndex = 0; outerIndex < aVector.size() - 1; outerIndex++)
		{
			int minIndex = outerIndex;
			for (int index = outerIndex + 1; index < aVector.size(); index++)
			{
				if (aVector[index] < aVector[minIndex])
				{
					minIndex = index;
				}
			}

			Swap(aVector[minIndex], aVector[outerIndex]);
		}
	}

	template<class T>
	void BubbleSort(std::vector<T>& aVector)
	{
		for (int outerIndex = 0; outerIndex < aVector.size() - 1; outerIndex++)
		{
			for (int index = 0; index < aVector.size() - outerIndex - 1; index++)
			{
				if (aVector[index + 1] < aVector[index])
				{
					Swap(aVector[index + 1], aVector[index]);
				}
			}
		}
	}

	template<class T>
	void QuickSort(std::vector<T>& aVector)
	{
		auto median = [](std::vector<T>& aVector, int aFrom, int aTo) -> T& {
			int middle = static_cast<int>((aTo + aFrom) * 0.5f);;

			if (aVector[middle] < aVector[aFrom])
			{
				Swap(aVector[middle], aVector[aFrom]);
			}
			if (aVector[aTo] < aVector[aFrom])
			{
				Swap(aVector[aTo], aVector[aFrom]);
			}
			if (aVector[middle] < aVector[aTo])
			{
				Swap(aVector[middle], aVector[aTo]);
			}

			return aVector[aTo];
		};

		auto partition = [&median](std::vector<T>& aVector, int aFrom, int aTo) {
			T& pivot = median(aVector, aFrom, aTo);
			int lowIndex = aFrom - 1;

			for (int index = aFrom; index < aTo; index++)
			{
				if (aVector[index] < pivot)
				{
					++lowIndex;
					Swap(aVector[lowIndex], aVector[index]);
				}
			}
			++lowIndex;
			Swap(aVector[lowIndex], aVector[aTo]);
			return lowIndex;
		};

		std::function<void(std::vector<T>&, int, int)> quickSort = [&partition, &quickSort](std::vector<T>& aVector, int aFrom, int aTo) {
			while (aFrom < aTo)
			{
				if (aTo - aFrom + 1 < 32) // Using insertionsort when size is small
				{
					for (size_t i = aFrom + 1; i <= aTo; i++)
					{
						T comparison = aVector[i];
						size_t index = i;
						while (index > aFrom && comparison < aVector[index - 1])
						{
							aVector[index] = aVector[index - 1];
							--index;
						}
						aVector[index] = comparison;
					}
					break;
				}
				else
				{
					int pivot = partition(aVector, aFrom, aTo);

					if (pivot - aFrom < aTo - pivot) // Sort the smaller part first
					{
						quickSort(aVector, aFrom, pivot - 1);
						aFrom = pivot + 1;
					}
					else
					{
						quickSort(aVector, pivot + 1, aTo);
						aTo = pivot - 1;
					}
				}
			}
		};

		quickSort(aVector, 0, static_cast<int>(aVector.size()) - 1);
	}

	template<class T, class U>
	void QuickSort(std::vector<T>& aVector, std::function<bool(U, U)> aCompareFunction)
	{
		auto median = [&aCompareFunction](std::vector<T>& aVector, int aFrom, int aTo) -> T& {
			int middle = static_cast<int>((aTo + aFrom) * 0.5f);;

			
			if (aCompareFunction(aVector[middle], aVector[aFrom]))
			{
				Swap(aVector[middle], aVector[aFrom]);
			}
			if (aCompareFunction(aVector[aTo], aVector[aFrom]))
			{
				Swap(aVector[aTo], aVector[aFrom]);
			}
			if (aCompareFunction(aVector[middle], aVector[aTo]))
			{
				Swap(aVector[middle], aVector[aTo]);
			}

			return aVector[aTo];
		};

		auto partition = [&median, &aCompareFunction](std::vector<T>& aVector, int aFrom, int aTo) {
			T& pivot = median(aVector, aFrom, aTo);
			int lowIndex = aFrom - 1;

			for (int index = aFrom; index < aTo; index++)
			{
				if (aCompareFunction(aVector[index], pivot))
				{
					++lowIndex;
					Swap(aVector[lowIndex], aVector[index]);
				}
			}
			++lowIndex;
			Swap(aVector[lowIndex], aVector[aTo]);
			return lowIndex;
		};

		std::function<void(std::vector<T>&, int, int)> quickSort = [&partition, &quickSort, &aCompareFunction](std::vector<T>& aVector, int aFrom, int aTo) {
			while (aFrom < aTo)
			{
				if (aTo - aFrom + 1 < 32) // Using insertionsort when size is small
				{
					for (int i = aFrom + 1; i <= aTo; i++)
					{
						T comparison = aVector[i];
						int index = i;
						while (index > aFrom && aCompareFunction(comparison, aVector[index - 1]))
						{
							aVector[index] = aVector[index - 1];
							--index;
						}
						aVector[index] = comparison;
					}
					break;
				}
				else
				{
					int pivot = partition(aVector, aFrom, aTo);

					if (pivot - aFrom < aTo - pivot) // Sort the smaller part first
					{
						quickSort(aVector, aFrom, pivot - 1);
						aFrom = pivot + 1;
					}
					else
					{
						quickSort(aVector, pivot + 1, aTo);
						aTo = pivot - 1;
					}
				}
			}
		};

		quickSort(aVector, 0, static_cast<int>(aVector.size()) - 1);
	}

	template<class T>
	void MergeSort(std::vector<T>& aVector)
	{
		auto merge = [](std::vector<T>& aVector, int aStart, int aMiddle, int anEnd) {
			std::vector<T> left;
			std::vector<T> right;

			auto begin = aVector.begin();
			auto middle = begin + aMiddle + 1;
			left.insert(left.end(), begin + aStart, middle);
			right.insert(right.end(), middle, begin + anEnd + 1);

			int leftIndex = 0;
			int rightIndex = 0;
			int vectorIndex = aStart;

			while (leftIndex < left.size() && rightIndex < right.size())
			{
				if (left[leftIndex] < right[rightIndex])
				{
					aVector[vectorIndex] = left[leftIndex];
					++leftIndex;
				}
				else
				{
					aVector[vectorIndex] = right[rightIndex];
					++rightIndex;
				}
				++vectorIndex;
			}

			while (leftIndex < left.size())
			{
				aVector[vectorIndex] = left[leftIndex];
				++leftIndex;
				++vectorIndex;
			}

			while (rightIndex < right.size())
			{
				aVector[vectorIndex] = right[rightIndex];
				++rightIndex;
				++vectorIndex;
			}
		};

		std::function<void(std::vector<T>&, int, int)> mergeSort = [&merge, &mergeSort](std::vector<T>& aVector, int aStart, int anEnd) {
			if (aStart < anEnd)
			{
				int middle = static_cast<int>((aStart + anEnd) * 0.5f);

				mergeSort(aVector, aStart, middle);
				mergeSort(aVector, middle + 1, anEnd);

				merge(aVector, aStart, middle, anEnd);
			}
		};

		mergeSort(aVector, 0, static_cast<int>(aVector.size()) - 1);
	}

	template<class T>
	void HeapSort(std::vector<T>& aVector)
	{
		std::function<void(std::vector<T>&, int)> heapify = [&heapify](std::vector<T>& aVector, int aRoot) {
			int largest = aRoot;
			int left = 2 * aRoot + 1;
			int right = 2 * aRoot + 2;

			if (left < aVector.size() && aVector[largest] < aVector[left])
			{
				largest = left;
			}
			if (right < aVector.size() && aVector[largest] < aVector[right])
			{
				largest = right;
			}

			if (largest != aRoot)
			{
				Swap(aVector[aRoot], aVector[largest]);
				heapify(aVector, largest);
			}
		};

		for (int index = aVector.size() * 0.5f - 1; index >= 0; index--)
		{
			heapify(aVector, index);
		}

		for (int index = aVector.size() - 1; index >= 0; index--)
		{
			Swap(aVector[0], aVector[index]);
			heapify(aVector, index);
		}
	}

	template<class T>
	void ShellSort(std::vector<T>& aVector)
	{
		for (int interval = static_cast<int>(aVector.size() * 0.5f); interval > 0; interval = static_cast<int>(interval * 0.5f))
		{
			for (int index = interval; index < aVector.size(); index++)
			{
				T temp = aVector[index];
				int i = index;
				for (int compare = i - interval; i >= interval && temp < aVector[compare]; i -= interval)
				{
					aVector[i] = aVector[compare];
				}
				aVector[i] = temp;
			}
		}
	}

	template<class T>
	void InsertionSort(std::vector<T>& aVector)
	{
		for (int i = 1; i < aVector.size(); i++)
		{
			T comparison = aVector[i];
			int index = i - 1;
			while (index >= 0 && comparison < aVector[index])
			{
				aVector[index + 1] = aVector[index];
				--index;
			}
			aVector[index + 1] = comparison;
		}
	}
}