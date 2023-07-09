#pragma once
#include <vector>

namespace CommonUtilities
{
	template <class T>class BSTSet;

	template <class T>
	class BSTSetNode
	{
	public:
		~BSTSetNode();
		BSTSetNode(const BSTSetNode&) = delete;
		BSTSetNode& operator=(const BSTSetNode&) = delete;

		const T& GetData() const;
		const BSTSetNode<T>* GetLeft() const;
		const BSTSetNode<T>* GetRight() const;

	private:
		T myValue;
		BSTSetNode* myLeft;
		BSTSetNode* myRight;

		friend class BSTSet<T>;
		BSTSetNode();
		BSTSetNode(const T& aValue);

		void ReplaceChild(BSTSetNode<T>* aChild, BSTSetNode<T>* aReplacement);
	};

	template <class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();

		const BSTSetNode<T>* GetRoot() const;

		bool HasElement(const T& aValue) const;
		void Insert(const T& aValue);
		void Remove(const T& aValue);
		void DSWBalance();

		std::vector<T> GetPreOrder() const;
		std::vector<T> GetInOrder() const;
		std::vector<T> GetPostOrder() const;
		std::vector<T> GetBreadthFirst() const;

	private:
		BSTSetNode<T>* myRoot;

		bool HasElement(const T& aValue, const BSTSetNode<T>* aNode) const;
		void Insert(const T& aValue, BSTSetNode<T>* aNode);
		BSTSetNode<T>* Remove(const T& aValue, BSTSetNode<T>* aNode);

		void PreOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const;
		void InOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const;
		void PostOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const;
		void BreadthFirst(BSTSetNode<T>* aNode, unsigned int aDepth, std::vector<T>& anOutResult) const;

		unsigned int GetDepth(BSTSetNode<T>* aNode) const;
		bool IsEqual(const T& aFirst, const T& aSecond) const;

		int CreateBackBone(BSTSetNode<T>* aPsuedoRoot);
		void BalanceBackBone(BSTSetNode<T>* aNode, int aSize);
		void Compress(BSTSetNode<T>* aNode, int aCount);
		int FullSize(int aSize) const;
	};

	template<class T>
	inline BSTSet<T>::BSTSet() : myRoot(nullptr)
	{
	}

	template<class T>
	inline BSTSet<T>::~BSTSet()
	{
		delete myRoot;
	}

	template<class T>
	inline const BSTSetNode<T>* BSTSet<T>::GetRoot() const
	{
		return myRoot;
	}

	template<class T>
	inline bool BSTSet<T>::HasElement(const T& aValue) const
	{
		if (myRoot != nullptr)
		{
			return HasElement(aValue, myRoot);
		}
		else
		{
			return false;
		}
	}

	template<class T>
	inline void BSTSet<T>::Insert(const T& aValue)
	{
		if (myRoot != nullptr)
		{
			Insert(aValue, myRoot);
		}
		else
		{
			myRoot = new BSTSetNode<T>(aValue);
		}
	}

	template<class T>
	inline void BSTSet<T>::Remove(const T& aValue)
	{
		if (myRoot != nullptr)
		{
			auto result = Remove(aValue, myRoot);
			if (result != myRoot)
			{
				delete myRoot;
				myRoot = result;
			}
		}
	}

	template<class T>
	inline void BSTSet<T>::DSWBalance()
	{
		BSTSetNode<T>* tempRoot = new BSTSetNode<T>();
		tempRoot->myRight = myRoot;
		int size = CreateBackBone(tempRoot);
		BalanceBackBone(tempRoot, size);
		myRoot = tempRoot->myRight;
		tempRoot->myRight = nullptr;
		delete tempRoot;
	}

	template<class T>
	inline std::vector<T> BSTSet<T>::GetPreOrder() const
	{
		std::vector<T> result;
		PreOrder(myRoot, result);
		return result;
	}

	template<class T>
	inline std::vector<T> BSTSet<T>::GetInOrder() const
	{
		std::vector<T> result;
		InOrder(myRoot, result);
		return result;
	}

	template<class T>
	inline std::vector<T> BSTSet<T>::GetPostOrder() const
	{
		std::vector<T> result;
		PostOrder(myRoot, result);
		return result;
	}

	template<class T>
	inline std::vector<T> BSTSet<T>::GetBreadthFirst() const
	{
		std::vector<T> result;
		unsigned int maxDepth = GetDepth(myRoot);
		for (unsigned int currentDepth = 1; currentDepth <= maxDepth; currentDepth++)
		{
			BreadthFirst(myRoot, currentDepth, result);
		}
		return result;
	}

	template<class T>
	inline bool BSTSet<T>::HasElement(const T& aValue, const BSTSetNode<T>* aNode) const
	{
		if (IsEqual(aValue, aNode->myValue))
		{
			return true;
		}
		else if (aValue < aNode->myValue)
		{
			if (aNode->myLeft != nullptr)
			{
				return HasElement(aValue, aNode->myLeft);
			}
		}
		else
		{
			if (aNode->myRight != nullptr)
			{
				return HasElement(aValue, aNode->myRight);
			}
		}

		return false;
	}

	template<class T>
	inline void BSTSet<T>::Insert(const T& aValue, BSTSetNode<T>* aNode)
	{
		if (IsEqual(aValue, aNode->myValue))
		{
			return;
		}
		else if (aValue < aNode->myValue)
		{
			if (aNode->myLeft != nullptr)
			{
				Insert(aValue, aNode->myLeft);
			}
			else
			{
				aNode->myLeft = new BSTSetNode<T>(aValue);
			}
		}
		else
		{
			if (aNode->myRight != nullptr)
			{
				Insert(aValue, aNode->myRight);
			}
			else
			{
				aNode->myRight = new BSTSetNode<T>(aValue);
			}
		}
	}

	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::Remove(const T& aValue, BSTSetNode<T>* aNode)
	{
		if (IsEqual(aValue, aNode->myValue))
		{
			if (aNode->myLeft == nullptr && aNode->myRight == nullptr)
			{
				return nullptr;
			}
			else if (aNode->myLeft == nullptr)
			{
				BSTSetNode<T>* temp = aNode->myRight;
				aNode->myRight = nullptr;
				return temp;
			}
			else if (aNode->myRight == nullptr)
			{
				BSTSetNode<T>* temp = aNode->myLeft;
				aNode->myLeft = nullptr;
				return temp;
			}
			else
			{
				BSTSetNode<T>* rightmostParent = aNode;
				BSTSetNode<T>* rightmost = aNode->myLeft;
				while (rightmost->myRight != nullptr)
				{
					rightmostParent = rightmost;
					rightmost = rightmost->myRight;
				}

				if (rightmostParent != aNode)
				{
					rightmostParent->myRight = rightmost->myLeft;

					rightmost->myLeft = aNode->myLeft;
					rightmost->myRight = aNode->myRight;
				}
				else
				{
					rightmost->myRight = aNode->myRight;
				}

				aNode->myLeft = nullptr;
				aNode->myRight = nullptr;

				return rightmost;
			}
		}
		else if (aValue < aNode->myValue)
		{
			if (aNode->myLeft != nullptr)
			{
				BSTSetNode<T>* result = Remove(aValue, aNode->myLeft);
				if (result != aNode->myLeft)
				{
					delete aNode->myLeft;
					aNode->myLeft = result;
				}
			}
		}
		else
		{
			if (aNode->myRight != nullptr)
			{
				BSTSetNode<T>* result = Remove(aValue, aNode->myRight);
				if (result != aNode->myRight)
				{
					delete aNode->myRight;
					aNode->myRight = result;
				}
			}
		}
		return aNode;
	}

	template<class T>
	inline void BSTSet<T>::PreOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const
	{
		if (aNode == nullptr)
		{
			return;
		}

		anOutResult.emplace_back(aNode->myValue);

		if (aNode->myLeft != nullptr)
		{
			PreOrder(aNode->myLeft, anOutResult);
		}

		if (aNode->myRight != nullptr)
		{
			PreOrder(aNode->myRight, anOutResult);
		}
	}

	template<class T>
	inline void BSTSet<T>::InOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const
	{
		if (aNode == nullptr)
		{
			return;
		}

		if (aNode->myLeft != nullptr)
		{
			InOrder(aNode->myLeft, anOutResult);
		}

		anOutResult.emplace_back(aNode->myValue);

		if (aNode->myRight != nullptr)
		{
			InOrder(aNode->myRight, anOutResult);
		}
	}

	template<class T>
	inline void BSTSet<T>::PostOrder(BSTSetNode<T>* aNode, std::vector<T>& anOutResult) const
	{
		if (aNode == nullptr)
		{
			return;
		}

		if (aNode->myLeft != nullptr)
		{
			PostOrder(aNode->myLeft, anOutResult);
		}

		if (aNode->myRight != nullptr)
		{
			PostOrder(aNode->myRight, anOutResult);
		}

		anOutResult.emplace_back(aNode->myValue);
	}

	template<class T>
	inline void BSTSet<T>::BreadthFirst(BSTSetNode<T>* aNode, unsigned int aDepth, std::vector<T>& anOutResult) const
	{
		if (aNode == nullptr)
		{
			return;
		}

		if (aDepth == 1)
		{
			anOutResult.emplace_back(aNode->myValue);
		}
		else if (aDepth > 1)
		{
			BreadthFirst(aNode->myLeft, aDepth - 1, anOutResult);
			BreadthFirst(aNode->myRight, aDepth - 1, anOutResult);
		}
	}

	template<class T>
	inline unsigned int BSTSet<T>::GetDepth(BSTSetNode<T>* aNode) const
	{
		if (aNode == nullptr)
		{
			return 0;
		}

		unsigned int leftDepth = GetDepth(aNode->myLeft);
		unsigned int rightDepth = GetDepth(aNode->myRight);

		if (rightDepth < leftDepth)
		{
			return leftDepth + 1;
		}
		else
		{
			return rightDepth + 1;
		}
	}

	template<class T>
	inline bool BSTSet<T>::IsEqual(const T& aFirst, const T& aSecond) const
	{
		return !(aFirst < aSecond) && !(aSecond < aFirst);
	}

	template<class T>
	inline int BSTSet<T>::CreateBackBone(BSTSetNode<T>* aRoot)
	{
		auto tail = aRoot;
		auto remainder = aRoot->myRight;
		int size = 0;

		while (remainder != nullptr)
		{
			if (remainder->myLeft == nullptr)
			{
				tail = remainder;
				remainder = remainder->myRight;
				size++;
			}
			else
			{
				auto temp = remainder->myLeft;
				remainder->myLeft = temp->myRight;
				temp->myRight = remainder;
				remainder = temp;
				tail->myRight = temp;
			}
		}

		return size;
	}

	template<class T>
	inline void BSTSet<T>::BalanceBackBone(BSTSetNode<T>* aNode, int aSize)
	{
		int leaves = FullSize(aSize);
		Compress(aNode, aSize - leaves);
		while (leaves > 1)
		{
			leaves /= 2;
			Compress(aNode, leaves);
		}
	}

	template<class T>
	inline void BSTSet<T>::Compress(BSTSetNode<T>* aNode, int aCount)
	{
		auto scanner = aNode;
		for (int i = 0; i < aCount; i++)
		{
			auto child = scanner->myRight;
			scanner->myRight = child->myRight;
			scanner = scanner->myRight;
			child->myRight = scanner->myLeft;
			scanner->myLeft = child;
		}
	}

	template<class T>
	inline int BSTSet<T>::FullSize(int aSize) const
	{
		int result = 1;
		while (result <= aSize)
		{
			result = result + result + 1;
		}
		return result / 2;
	}

	template<class T>
	inline BSTSetNode<T>::BSTSetNode() : myValue(), myLeft(nullptr), myRight(nullptr)
	{
	}

	template<class T>
	inline BSTSetNode<T>::BSTSetNode(const T& aValue) : myValue(aValue), myLeft(nullptr), myRight(nullptr)
	{
	}

	template<class T>
	inline BSTSetNode<T>::~BSTSetNode()
	{
		delete myLeft;
		delete myRight;
	}

	template<class T>
	inline void BSTSetNode<T>::ReplaceChild(BSTSetNode<T>* aChild, BSTSetNode<T>* aReplacement)
	{
		if (aChild == myRight)
		{
			myRight = aReplacement;
		}
		else if (aChild == myLeft)
		{
			myLeft = aReplacement;
		}
	}

	template<class T>
	inline const T& BSTSetNode<T>::GetData() const
	{
		return myValue;
	}

	template<class T>
	const BSTSetNode<T>* CommonUtilities::BSTSetNode<T>::GetLeft() const
	{
		return myLeft;
	}
	template<class T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return myRight;
	}
}