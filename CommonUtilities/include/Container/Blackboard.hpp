#pragma once
#include <unordered_map>
#include <assert.h>
#include <typeinfo>	

namespace CommonUtilities
{
	template<typename key>
	class Blackboard
	{
	public:
		Blackboard();
		Blackboard(unsigned int aByteSize);
		~Blackboard() = default;

		bool HasValue(const key& aKey) const;

		// If true: the container has expanded and any pointers are invalidated
		template<typename value>
		bool SetValue(const key& aKey, const value& aValue);

		template<typename value>
		const value& GetValue(const key& aKey) const;

		template<typename value>
		value& ChangeValue(const key& aKey);

		template<typename value>
		const value* GetValueUnsafe(const key& aKey) const;

		template<typename value>
		value* ChangeValueUnsafe(const key& aKey);

		void Clear();

	private:
		unsigned int myCurrentIndex;
		unsigned long myCurrentSize;
		std::vector<char> myData;
		std::unordered_map<key, unsigned int> myDataPointers;
		std::unordered_map<key, const std::type_info*> myDataTypes;
	};

	template<typename key>
	Blackboard<key>::Blackboard(): myDataPointers(), myData(), myDataTypes(), myCurrentIndex(0), myCurrentSize(0)
	{
	}

	template<typename key>
	inline Blackboard<key>::Blackboard(unsigned int aByteSize) : myDataPointers(), myData(), myDataTypes(), myCurrentIndex(0), myCurrentSize(aByteSize)
	{
		myData.resize(aByteSize);
	}

	template<typename key>
	bool Blackboard<key>::HasValue(const key& aKey) const
	{
		auto type = myDataPointers.find(aKey);
		return (type != myDataPointers.end());
	}

	template<typename key>
	template<typename value>
	bool Blackboard<key>::SetValue(const key& aKey, const value& aValue)
	{		
		bool hasResized = false;
		auto it = myDataPointers.find(aKey);
		if (it == myDataPointers.end())
		{
			unsigned int valueSize = sizeof(value);
			if (myCurrentSize < myCurrentIndex + valueSize)
			{
				myCurrentSize = (myCurrentSize + valueSize) * 2;
				myData.resize(myCurrentSize);
				hasResized = true;
			}
			new (&myData[myCurrentIndex]) value(aValue);
			myDataPointers[aKey] = myCurrentIndex;

			myCurrentIndex += valueSize;
			myDataTypes[aKey] = &typeid(value);
		}
		else
		{
			assert(typeid(value) == *myDataTypes[aKey] && "Blackboard: Trying to set a value to a different type than it was created with!");
			*(reinterpret_cast<value*>(myData[it->second])) = aValue;
		}
		return hasResized;
	}

	template<typename key>
	template<typename value>
	const value& Blackboard<key>::GetValue(const key& aKey) const
	{
		auto type = myDataTypes.find(aKey);
		assert(type != myDataTypes.end() && "Key not found in blackboard!");
		assert(typeid(value) == *(type->second) && "Incorrect type!");

		auto it = myDataPointers.find(aKey);
		assert(it != myDataPointers.end() && "Key not found in blackboard!");
		return reinterpret_cast<const value&>(myData[it->second]);
	}

	template<typename key>
	template<typename value>
	value& Blackboard<key>::ChangeValue(const key& aKey)
	{
		return const_cast<value&>(GetValue<value>(aKey));;
	}

	template<typename key>
	template<typename value>
	inline const value* Blackboard<key>::GetValueUnsafe(const key& aKey) const
	{
		auto it = myDataPointers.find(aKey);
		assert(it != myDataPointers.end() && "Key not found in blackboard!");
		return reinterpret_cast<const value*>(&myData[it->second]);
	}

	template<typename key>
	template<typename value>
	inline value* Blackboard<key>::ChangeValueUnsafe(const key& aKey)
	{
		return const_cast<value*>(GetValueUnsafe<value>(aKey));
	}

	template<typename key>
	void Blackboard<key>::Clear()
	{
		myCurrentIndex = 0;
		myDataPointers.clear();
		myDataTypes.clear();
	}
}