#pragma once
#include "Vector4.hpp"
#include "External/jsonCpp/json.h"

namespace CommonUtilities
{
	template <>
	inline Vector2<float>::Vector2(Json::Value aJson) : x(aJson["X"].asFloat()), y(aJson["Y"].asFloat())
	{
	}

	template <>
	inline Vector2<double>::Vector2(Json::Value aJson) : x(aJson["X"].asDouble()), y(aJson["Y"].asDouble())
	{
	}

	template <>
	inline Vector2<int>::Vector2(Json::Value aJson) : x(aJson["X"].asInt()), y(aJson["Y"].asInt())
	{
	}

	template <>
	inline Vector2<unsigned>::Vector2(Json::Value aJson) : x(aJson["X"].asUInt()), y(aJson["Y"].asUInt())
	{
	}

	template <typename T>
	inline Vector2<T>::operator Json::Value() const
	{
		Json::Value result;
		result["X"] = x;
		result["Y"] = y;
		return result;
	}

	template <>
	inline Vector3<float>::Vector3(Json::Value aJson) : x(aJson["X"].asFloat()), y(aJson["Y"].asFloat()), z(aJson["Z"].asFloat())
	{
	}

	template <>
	inline Vector3<double>::Vector3(Json::Value aJson) : x(aJson["X"].asDouble()), y(aJson["Y"].asDouble()), z(aJson["Z"].asDouble())
	{
	}

	template <>
	inline Vector3<int>::Vector3(Json::Value aJson) : x(aJson["X"].asInt()), y(aJson["Y"].asInt()), z(aJson["Z"].asInt())
	{
	}

	template <>
	inline Vector3<unsigned>::Vector3(Json::Value aJson) : x(aJson["X"].asUInt()), y(aJson["Y"].asUInt()), z(aJson["Z"].asUInt())
	{
	}

	template <typename T>
	inline Vector3<T>::operator Json::Value() const
	{
		Json::Value result;
		result["X"] = x;
		result["Y"] = y;
		result["Z"] = z;
		return result;
	}

	template <>
	inline Vector4<float>::Vector4(Json::Value aJson) : x(aJson["X"].asFloat()), y(aJson["Y"].asFloat()), z(aJson["Z"].asFloat()), w(aJson["W"].asFloat())
	{
	}

	template <>
	inline Vector4<double>::Vector4(Json::Value aJson) : x(aJson["X"].asDouble()), y(aJson["Y"].asDouble()), z(aJson["Z"].asDouble()), w(aJson["W"].asDouble())
	{
	}

	template <>
	inline Vector4<int>::Vector4(Json::Value aJson) : x(aJson["X"].asInt()), y(aJson["Y"].asInt()), z(aJson["Z"].asInt()), w(aJson["W"].asInt())
	{
	}

	template <>
	inline Vector4<unsigned>::Vector4(Json::Value aJson) : x(aJson["X"].asUInt()), y(aJson["Y"].asUInt()), z(aJson["Z"].asUInt()), w(aJson["W"].asUInt())
	{
	}

	template <typename T>
	inline Vector4<T>::operator Json::Value() const
	{
		Json::Value result;
		result["X"] = x;
		result["Y"] = y;
		result["Z"] = z;
		result["W"] = w;
		return result;
	}
}
