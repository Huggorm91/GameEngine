#include "MemoryBlock.h"

namespace Crimson
{
	MemoryBlock::MemoryBlock() :
		myCurrentIndex(0),
		myCurrentSize(0)
	{}

	MemoryBlock::MemoryBlock(const size_t& aByteSize) :
		myCurrentIndex(0),
		myCurrentSize(aByteSize)
	{
		myData.resize(aByteSize);
	}

	size_t MemoryBlock::Allocate(const size_t& aSize)
	{
		if (myCurrentSize < myCurrentIndex + aSize)
		{
			myCurrentSize = (myCurrentSize + aSize) * 2;
			myData.resize(myCurrentSize);
		}
		myDataIndices.emplace_back(myCurrentIndex);

		size_t oldIndex = myCurrentIndex;
		myCurrentIndex += aSize;

		return oldIndex;
	}

	size_t MemoryBlock::AddValue(const void* aValue, const size_t& aSize)
	{
		if (myCurrentSize < myCurrentIndex + aSize)
		{
			myCurrentSize = (myCurrentSize + aSize) * 2;
			myData.resize(myCurrentSize);
		}
		memcpy_s(&myData[myCurrentIndex], myCurrentSize - myCurrentIndex, aValue, aSize);

		myDataIndices.emplace_back(myCurrentIndex);

		size_t oldIndex = myCurrentIndex;
		myCurrentIndex += aSize;

		return oldIndex;
	}

	void MemoryBlock::Clear()
	{
		myCurrentIndex = 0;
		myDataIndices.clear();
		memset(myData.data(), 0, myData.size());
	}

	void* MemoryBlock::GetVoid(const size_t& anIndex)
	{
		assert(anIndex < myCurrentIndex && "Index out of range!");
		return &myData[anIndex];
	}

	void MemoryBlock::Resize(const size_t& aByteSize)
	{
		if (myCurrentSize < aByteSize)
		{
			myData.resize(aByteSize);
			myCurrentSize = aByteSize;
		}
	}

	size_t MemoryBlock::GetCount() const
	{
		return myDataIndices.size();
	}

	size_t MemoryBlock::GetSize() const
	{
		return myCurrentIndex;
	}

	size_t MemoryBlock::GetCapacity() const
	{
		return myCurrentSize;
	}
}