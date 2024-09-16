#include "MemoryBlock.h"

namespace Crimson
{
	MemoryBlock::MemoryBlock() :
    myCurrentIndex(0),
    myCurrentSize(0)
{
}

MemoryBlock::MemoryBlock(const size_t& aByteSize) :
    myCurrentIndex(0),
    myCurrentSize(aByteSize)
{
    myData.resize(aByteSize);
}

std::pair<size_t, bool> MemoryBlock::Allocate(const size_t& aSize)
{
    bool hasResized = false;
    if(myCurrentSize < myCurrentIndex + aSize)
    {
        myCurrentSize = (myCurrentSize + aSize) * 2;
        myData.resize(myCurrentSize);
        hasResized = true;
    }
    myDataIndices.emplace_back(myCurrentIndex);

    size_t oldIndex = myCurrentIndex;
    myCurrentIndex += aSize;

    return {oldIndex, hasResized};
}

std::pair<size_t, bool> MemoryBlock::AddValue(const void* aValue, const size_t& aSize)
{
    bool hasResized = false;
    if(myCurrentSize < myCurrentIndex + aSize)
    {
        myCurrentSize = (myCurrentSize + aSize) * 2;
        myData.resize(myCurrentSize);
        hasResized = true;
    }
    std::memcpy(&myData[myCurrentIndex], aValue, aSize);

    myDataIndices.emplace_back(myCurrentIndex);

    size_t oldIndex = myCurrentIndex;
    myCurrentIndex += aSize;

    return {oldIndex, hasResized};
}

void MemoryBlock::Clear()
{
    myCurrentIndex = 0;
    myDataIndices.clear();
    std::fill(myData.begin(), myData.end(), std::byte{0});
}

void* MemoryBlock::GetVoid(const size_t& anIndex)
{
    assert(anIndex < myCurrentIndex && "Index out of range!");
    return &myData[anIndex];
}

void MemoryBlock::Resize(const size_t& aByteSize)
{
    if(myCurrentSize < aByteSize)
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