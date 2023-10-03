#pragma once
#include "External/SpookyHash/SpookyHashV2.h"
#include "External/CityHash/city.h"
#include <string>

namespace Crimson
{
	template <class T>
	uint64_t Hash64(const T& aKey)
	{
		return SpookyHash::Hash64(&aKey, sizeof(aKey), 123);
	}

	template <>
	uint64_t Hash64(const std::string& aKey)
	{
		const char* cString = aKey.c_str();
		return SpookyHash::Hash64(cString, sizeof(char) * aKey.size(), 123);
	}

	template <class T>
	uint32_t Hash32(const T& aKey)
	{
		return SpookyHash::Hash32(&aKey, sizeof(aKey), 123);
	}

	template <>
	uint32_t Hash32(const std::string& aKey)
	{
		const char* cString = aKey.c_str();
		return SpookyHash::Hash32(cString, sizeof(char) * aKey.size(), 123);
	}

	/*template <class T>
	uint64_t Hash64(const T& aKey)
	{
		const char* data = (const char*) &aKey;
		return CityHash64(data, sizeof(aKey));
	}

	template <>
	uint64_t Hash64(const std::string& aKey)
	{
		const char* cString = aKey.c_str();
		return CityHash64(cString, sizeof(cString));
	}

	template <class T>
	uint32_t Hash32(const T& aKey)
	{
		const char* data = (const char*) &aKey;
		return CityHash32(data, sizeof(aKey));
	}

	template <>
	uint32_t Hash32(const std::string& aKey)
	{
		const char* cString = aKey.c_str();
		return CityHash32(cString, sizeof(cString));
	}*/
}