#pragma once
#include <vector>
#include <assert.h>

namespace Crimson
{
// Just a chunk of memory. Does not typecheck anything! So user has to ensure they are using valid types.
	class MemoryBlock
	{
	public:
		MemoryBlock();
		MemoryBlock(const size_t& aByteSize);
		~MemoryBlock() = default;

		size_t Allocate(const size_t& aSize);

		size_t AddValue(const void* aValue, const size_t& aSize);
		template<typename value>
		size_t AddValue(const value& aValue);
		template<typename value>
		size_t AddValue(value&& aValue);

		template<typename value>
		const value& GetValue(const size_t& anIndex) const;
		template<typename value>
		value& GetValue(const size_t& anIndex);

		void* GetVoid(const size_t& anIndex);

		// Can only increase the size
		void Resize(const size_t& aByteSize);

		void Clear();

		// Returns amount of times you have allocated memory inside the block
		size_t GetCount() const;

		// Returns size of currently used data
		size_t GetSize() const;
		// Returns size of total amount of allocated memory
		size_t GetCapacity() const;

	private:
		size_t myCurrentIndex;
		size_t myCurrentSize;
		std::vector<size_t> myDataIndices;
		std::vector<char> myData;
	};

	template<typename value>
	inline size_t MemoryBlock::AddValue(const value& aValue)
	{
		unsigned int valueSize = sizeof(value);
		if (myCurrentSize < myCurrentIndex + valueSize)
		{
			myCurrentSize = (myCurrentSize + valueSize) * 2;
			myData.resize(myCurrentSize);
		}
		new (&myData[myCurrentIndex]) value(aValue);

		myDataIndices.emplace_back(myCurrentIndex);

		size_t oldIndex = myCurrentIndex;
		myCurrentIndex += valueSize;

		return oldIndex;
	}

	template<typename value>
	inline size_t MemoryBlock::AddValue(value&& aValue)
	{
		unsigned int valueSize = sizeof(value);
		if (myCurrentSize < myCurrentIndex + valueSize)
		{
			myCurrentSize = (myCurrentSize + valueSize) * 2;
			myData.resize(myCurrentSize);
		}
		new (&myData[myCurrentIndex]) value(std::move(aValue));

		myDataIndices.emplace_back(myCurrentIndex);

		size_t oldIndex = myCurrentIndex;
		myCurrentIndex += valueSize;

		return oldIndex;
	}

	template<typename value>
	inline const value& MemoryBlock::GetValue(const size_t& anIndex) const
	{
		assert(anIndex < myCurrentIndex && "Index out of range!");
		assert(anIndex + sizeof(value) <= myCurrentSize && "Trying to access memory out of range");
		return reinterpret_cast<const value&>(myData[anIndex]);
	}

	template<typename value>
	inline value& MemoryBlock::GetValue(const size_t& anIndex)
	{
		assert(anIndex < myCurrentIndex && "Index out of range!");
		assert(anIndex + sizeof(value) <= myCurrentSize && "Trying to access memory out of range");
		return reinterpret_cast<value&>(myData[anIndex]);
	}
}