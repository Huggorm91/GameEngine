#pragma once
#include <vector>
#include "../Hash.hpp"
#include "../UtilityFunctions.hpp"

namespace CommonUtilities
{
	template <class Key, class Value>
	class HashMap
	{
	public:
		HashMap(int aCapacity);

		bool Insert(const Key& aKey, const Value& aValue);
		bool Remove(const Key& aKey);

		// Returns nullptr if aKey does not exist
		const Value* Get(const Key& aKey) const;
		// Returns nullptr if aKey does not exist
		Value* Get(const Key& aKey);

	private:
		enum State : uint16_t
		{
			Empty = 1 << 0,
			InUse = 1 << 1,
			Removed = 1 << 2
		};
		template <class Key, class Value>
		struct Entry
		{
			State state;
			Key key;
			Value value;

			Entry() : state(Empty), key(), value() {}
		};
		std::vector<Entry<Key, Value>> myData;
		size_t myCount;

		const Entry<Key, Value>* FindKey(const Key& aKey) const;
		Entry<Key, Value>* FindKey(const Key& aKey);
		Entry<Key, Value>* FindInsertionPoint(const Key& aKey);

		void Resize();
	};

	template<class Key, class Value>
	inline HashMap<Key, Value>::HashMap(int aCapacity) : myData(), myCount(0)
	{
		myData.reserve(aCapacity);
		for (int i = 0; i < aCapacity; i++)
		{
			myData.emplace_back(Entry<Key, Value>());
		}
	}

	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		auto entry = FindInsertionPoint(aKey);
		if (entry == nullptr)
		{
			return false;
		}
		if (entry->state ^ InUse) // works as bitwise not equal
		{
			++myCount;
		}		

		entry->state = InUse;
		entry->key = aKey;
		entry->value = aValue;

		if (static_cast<float>(myCount) / myData.size() > 0.75f)
		{
			Resize();
		}
		return true;
	}

	template<class Key, class Value>
	inline bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		auto result = FindKey(aKey);
		if (result != nullptr)
		{
			result->state = Removed;
			--myCount;
			return true;
		}
		return false;
	}

	template<class Key, class Value>
	inline const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		const Entry<Key, Value>* result = FindKey(aKey);
		if (result != nullptr)
		{
			return &result->value;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		Entry<Key, Value>* result = FindKey(aKey);
		if (result != nullptr)
		{
			return &result->value;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline const HashMap<Key, Value>::Entry<Key, Value>* HashMap<Key, Value>::FindKey(const Key& aKey) const
	{
		int size = myData.size();
		if (size <= 0)
		{
			return nullptr;
		}

		uint32_t hash = Hash32(aKey);
		int index = hash % size;

		for (int i = 1; i < size; i++)
		{
			const Entry<Key, Value>* currentEntry = &myData[index];
			if (currentEntry->state & Empty) // works as bitwise equals
			{
				return nullptr;
			}
			else if (currentEntry->state & InUse && IsEqual(currentEntry->key, aKey))
			{
				return currentEntry;
			}
			index = (hash + i * i) % size;
		}
		return nullptr;
	}

	template<class Key, class Value>
	inline HashMap<Key, Value>::Entry<Key, Value>* HashMap<Key, Value>::FindKey(const Key& aKey)
	{
		return const_cast<Entry<Key, Value>*>(const_cast<const HashMap*>(this)->FindKey(aKey));;
	}

	template<class Key, class Value>
	inline HashMap<Key, Value>::Entry<Key, Value>* HashMap<Key, Value>::FindInsertionPoint(const Key& aKey)
	{
		int size = myData.size();
		if (size <= 0)
		{
			return nullptr;
		}

		Entry<Key, Value>* firstRemoved = nullptr;
		uint32_t hash = Hash32(aKey);
		int index = hash % size;
		for (int i = 1; i <= size; i++)
		{
			Entry<Key, Value>* currentEntry = &myData[index];
			if (currentEntry->state & Empty || IsEqual(currentEntry->key, aKey))
			{
				return currentEntry;
			}
			else if (firstRemoved == nullptr && currentEntry->state & Removed) // works as bitwise equals
			{
				firstRemoved = currentEntry;
			}
			index = (hash + i * i) % size;
		}

		return firstRemoved;
	}

	template<class Key, class Value>
	inline void HashMap<Key, Value>::Resize()
	{
		auto temp = myData;
		size_t newSize = myData.size() *2;
		myCount = 0;
		myData.clear();
		myData.reserve(newSize);

		for (int i = 0; i < newSize; i++)
		{
			myData.emplace_back(Entry<Key, Value>());
		}

		for (auto& entry : temp)
		{
			if (entry.state & InUse) // works as bitwise equals
			{
				Insert(entry.key, entry.value);
			}
		}
	}
}