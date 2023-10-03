#pragma once
#include <vector>
#include <assert.h>

namespace Crimson
{
	template <class T>
	class Queue
	{
	public:
		Queue();

		int GetSize() const;
		const T& GetFront() const;
		T& GetFront();

		void Enqueue(const T& aValue);
		T Dequeue();

	private:
		std::vector<T> myQueue;
		int myFirst;
	};

	template<class T>
	inline Queue<T>::Queue(): myQueue(), myFirst(0)
	{
	}

	template<class T>
	inline int Queue<T>::GetSize() const
	{
		return static_cast<int>(myQueue.size() - myFirst);
	}

	template<class T>
	inline const T& Queue<T>::GetFront() const
	{
		assert(myFirst < myQueue.size() && myQueue.empty() == false);
		return myQueue[myFirst];
	}

	template<class T>
	inline T& Queue<T>::GetFront()
	{
		assert(myFirst < myQueue.size() && myQueue.empty() == false);
		return myQueue[myFirst];
	}

	template<class T>
	inline void Queue<T>::Enqueue(const T& aValue)
	{
		myQueue.emplace_back(aValue);
	}

	template<class T>
	inline T Queue<T>::Dequeue()
	{
		assert(myFirst < myQueue.size() && myQueue.empty() == false);
		T dequeueElement = myQueue[myFirst];
		++myFirst;
		if (myFirst == 100)
		{
			auto iter = myQueue.begin() + 100;
			myQueue.erase(myQueue.begin(), iter);
			myFirst = 0;
		}
		return dequeueElement;
	}
}