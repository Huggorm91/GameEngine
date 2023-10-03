#pragma once
namespace Crimson
{
	template <class T> class DoublyLinkedListNode;

	template <class T>
	class DoublyLinkedList
	{
	public:
		DoublyLinkedList();
		~DoublyLinkedList();

		int GetSize() const;
		DoublyLinkedListNode<T>* GetFirst();
		DoublyLinkedListNode<T>* GetLast();

		void InsertFirst(const T& aValue);
		void InsertLast(const T& aValue);
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);

		void Remove(DoublyLinkedListNode<T>* aNode);
		bool RemoveFirst(const T& aValue);
		bool RemoveLast(const T& aValue);

		DoublyLinkedListNode<T>* FindFirst(const T& aValue);
		DoublyLinkedListNode<T>* FindLast(const T& aValue);

	private:
		int mySize;
		DoublyLinkedListNode<T>* myFirst;
		DoublyLinkedListNode<T>* myLast;
	};

	template <class T>
	class DoublyLinkedListNode
	{
	public:
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;

		const T& GetValue() const;
		T& GetValue();

		DoublyLinkedListNode<T>* GetNext() const;
		DoublyLinkedListNode<T>* GetPrevious() const;
	private:
		T myData;
		DoublyLinkedListNode<T>* myNext;
		DoublyLinkedListNode<T>* myPrevious;

		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode() = default;
	};

	template<class T>
	inline DoublyLinkedList<T>::DoublyLinkedList() : mySize(0), myFirst(nullptr), myLast(nullptr)
	{
	}

	template<class T>
	inline DoublyLinkedList<T>::~DoublyLinkedList()
	{
		auto node = myFirst;
		while (node != nullptr)
		{
			myFirst = node->GetNext();
			delete node;
			node = myFirst;
		}
	}

	template<class T>
	inline int DoublyLinkedList<T>::GetSize() const
	{
		return mySize;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myFirst;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		return myLast;
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		if (mySize == 0)
		{
			myLast = newNode;
		}
		else
		{
			myFirst->myPrevious = newNode;
			newNode->myNext = myFirst;
		}
		myFirst = newNode;
		++mySize;
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		if (mySize == 0)
		{
			myFirst = newNode;
		}
		else
		{
			myLast->myNext = newNode;
			newNode->myPrevious = myLast;
		}
		myLast = newNode;
		++mySize;
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		if (aNode == myFirst)
		{
			InsertFirst(aValue);
			return;
		}
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		newNode->myNext = aNode;
		newNode->myPrevious = aNode->myPrevious;
		newNode->myPrevious->myNext = newNode;
		aNode->myPrevious = newNode;
		++mySize;
	}

	template<class T>
	inline void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		if (aNode == myLast)
		{
			InsertLast(aValue);
			return;
		}
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		newNode->myPrevious = aNode;
		newNode->myNext = aNode->myNext;
		newNode->myNext->myPrevious = newNode;
		aNode->myNext = newNode;
		++mySize;
	}

	template<class T>
	inline void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		if (aNode == nullptr)
		{
			return;
		}

		if (aNode == myFirst)
		{
			myFirst = aNode->myNext;
		}
		else
		{
			aNode->myPrevious->myNext = aNode->myNext;
		}

		if (aNode == myLast)
		{
			myLast = aNode->myPrevious;
		}
		else
		{
			aNode->myNext->myPrevious = aNode->myPrevious;
		}

		--mySize;
		delete aNode;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		auto node = myFirst;
		while (node != nullptr)
		{
			if (node->GetValue() == aValue)
			{
				return node;
			}
			node = node->GetNext();
		}
		return nullptr;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		auto node = myLast;
		while (node != nullptr)
		{
			if (node->GetValue() == aValue)
			{
				return node;
			}
			node = node->GetPrevious();
		}
		return nullptr;
	}

	template<class T>
	inline bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		auto node = FindFirst(aValue);
		if (node != nullptr)
		{
			Remove(node);
			return true;
		}
		return false;
	}

	template<class T>
	inline bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		auto node = FindLast(aValue);
		if (node != nullptr)
		{
			Remove(node);
			return true;
		}
		return false;
	}

	template<class T>
	inline const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myData;
	}

	template<class T>
	inline T& DoublyLinkedListNode<T>::GetValue()
	{
		return myData;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}

	template<class T>
	inline DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrevious;
	}

	template<class T>
	inline DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue) : myNext(nullptr), myPrevious(nullptr), myData(aValue)
	{
	}
}