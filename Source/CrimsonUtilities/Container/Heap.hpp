#pragma once
#include <vector>
#include <assert.h>

namespace Crimson
{
	template <class T>
	class Heap
	{
	public:
		Heap() = default;
		~Heap() = default;

		int GetSize() const;
		const T& GetTop() const;

		void Enqueue(const T& aElement);
		T Dequeue();

		void Clear();

	private:
		std::vector<T> myData;

		void Swap(T& aFirst, T& aSecond);
	};

	template<class T>
	inline int Heap<T>::GetSize() const
	{
		return static_cast<int>(myData.size());
	}

	template<class T>
	inline const T& Heap<T>::GetTop() const
	{
		assert(myData.size() != 0);
		return myData.front();
	}

	template<class T>
	inline void Heap<T>::Enqueue(const T& aElement)
	{
		myData.emplace_back(aElement);
		int index = static_cast<int>(myData.size()) - 1;
		if (index > 0)
		{
			int parentIndex = (index - 1) / 2;
			while (myData[parentIndex] < myData[index])
			{
				Swap(myData[parentIndex], myData[index]);

				index = parentIndex;
				parentIndex = (index - 1) / 2;
			}
		}
	}

	template<class T>
	inline T Heap<T>::Dequeue()
	{
		assert(myData.size() != 0);
		T result = myData.front();
		myData.front() = myData.back();
		myData.pop_back();

		int index = 0;
		bool isSorting = true;
		while (isSorting)
		{
			int firstChildIndex = (2 * index) + 1;
			int secondChildIndex = firstChildIndex + 1;

			if (secondChildIndex < myData.size() && myData[firstChildIndex] < myData[secondChildIndex] && myData[index] < myData[secondChildIndex])
			{
				Swap(myData[index], myData[secondChildIndex]);
				index = secondChildIndex;
			}
			else if (firstChildIndex < myData.size() && myData[index] < myData[firstChildIndex])
			{
				Swap(myData[index], myData[firstChildIndex]);
				index = firstChildIndex;
			}
			else
			{
				isSorting = false;
			}
		}

		return result;
	}

	template<class T>
	inline void Heap<T>::Clear()
	{
		myData.clear();
	}

	template<class T>
	inline void Heap<T>::Swap(T& aFirst, T& aSecond)
	{
		T temp = aFirst;
		aFirst = aSecond;
		aSecond = temp;
	}
}