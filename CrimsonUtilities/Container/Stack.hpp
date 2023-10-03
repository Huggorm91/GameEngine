#pragma once
#include <vector>
#include <assert.h>

namespace Crimson
{
	template <class T>
	class Stack
	{
	public:
		Stack();

		int GetSize() const;
		const T& GetTop() const;
		T& GetTop();

		void Push(const T& aValue);
		T Pop();

	private:
		std::vector<T> myStack;
	};

	template<class T>
	inline Stack<T>::Stack(): myStack()
	{
	}

	template<class T>
	inline int Stack<T>::GetSize() const
	{
		return static_cast<int>(myStack.size());
	}

	template<class T>
	inline const T& Stack<T>::GetTop() const
	{
		assert(myStack.empty() == false);
		return myStack.back();
	}

	template<class T>
	inline T& Stack<T>::GetTop()
	{
		assert(myStack.empty() == false);
		return myStack.back();
	}

	template<class T>
	inline void Stack<T>::Push(const T& aValue)
	{
		myStack.emplace_back(aValue);
	}

	template<class T>
	inline T Stack<T>::Pop()
	{
		assert(myStack.empty() == false);

		T poppedElement = myStack.back();
		myStack.pop_back();
		return poppedElement;
	}
}