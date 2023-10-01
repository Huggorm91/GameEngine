#pragma once
#include "External/jsonCpp/json.h"
#include "Vector4.hpp"

namespace CommonUtilities
{
	template <>
	inline Vector2<float>::Vector2(Json::Value aJson) : 
		x(aJson["X"].isNull() ? aJson["R"].asFloat() : aJson["X"].asFloat()), 
		y(aJson["Y"].isNull() ? aJson["G"].asFloat() : aJson["Y"].asFloat())
	{
	}

	template <>
	inline Vector2<double>::Vector2(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asDouble() : aJson["X"].asDouble()),
		y(aJson["Y"].isNull() ? aJson["G"].asDouble() : aJson["Y"].asDouble())
	{
	}

	template <>
	inline Vector2<int>::Vector2(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asInt() : aJson["X"].asInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asInt() : aJson["Y"].asInt())
	{
	}

	template <>
	inline Vector2<unsigned>::Vector2(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asUInt() : aJson["X"].asUInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asUInt() : aJson["Y"].asUInt())
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

	template <typename T>
	inline Json::Value Vector2<T>::ToJsonColor() const
	{
		Json::Value result;
		result["R"] = x;
		result["G"] = y;
		return result;
	}

	template <>
	inline Vector3<float>::Vector3(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asFloat() : aJson["X"].asFloat()),
		y(aJson["Y"].isNull() ? aJson["G"].asFloat() : aJson["Y"].asFloat()), 
		z(aJson["Z"].isNull() ? aJson["B"].asFloat() : aJson["Z"].asFloat())
	{
	}

	template <>
	inline Vector3<double>::Vector3(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asDouble() : aJson["X"].asDouble()),
		y(aJson["Y"].isNull() ? aJson["G"].asDouble() : aJson["Y"].asDouble()),
		z(aJson["Z"].isNull() ? aJson["B"].asDouble() : aJson["Z"].asDouble())
	{
	}

	template <>
	inline Vector3<int>::Vector3(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asInt() : aJson["X"].asInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asInt() : aJson["Y"].asInt()),
		z(aJson["Z"].isNull() ? aJson["B"].asInt() : aJson["Z"].asInt())
	{
	}

	template <>
	inline Vector3<unsigned>::Vector3(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asUInt() : aJson["X"].asUInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asUInt() : aJson["Y"].asUInt()),
		z(aJson["Z"].isNull() ? aJson["B"].asUInt() : aJson["Z"].asUInt())
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

	template <typename T>
	inline Json::Value Vector3<T>::ToJsonColor() const
	{
		Json::Value result;
		result["R"] = x;
		result["G"] = y;
		result["B"] = z;
		return result;
	}

	template <>
	inline Vector4<float>::Vector4(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asFloat() : aJson["X"].asFloat()),
		y(aJson["Y"].isNull() ? aJson["G"].asFloat() : aJson["Y"].asFloat()),
		z(aJson["Z"].isNull() ? aJson["B"].asFloat() : aJson["Z"].asFloat()), 
		w(aJson["W"].isNull() ? aJson["A"].asFloat() : aJson["W"].asFloat())
	{
	}

	template <>
	inline Vector4<double>::Vector4(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asDouble() : aJson["X"].asDouble()),
		y(aJson["Y"].isNull() ? aJson["G"].asDouble() : aJson["Y"].asDouble()),
		z(aJson["Z"].isNull() ? aJson["B"].asDouble() : aJson["Z"].asDouble()),
		w(aJson["W"].isNull() ? aJson["A"].asDouble() : aJson["W"].asDouble())
	{
	}

	template <>
	inline Vector4<int>::Vector4(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asInt() : aJson["X"].asInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asInt() : aJson["Y"].asInt()),
		z(aJson["Z"].isNull() ? aJson["B"].asInt() : aJson["Z"].asInt()),
		w(aJson["W"].isNull() ? aJson["A"].asInt() : aJson["W"].asInt())
	{
	}

	template <>
	inline Vector4<unsigned>::Vector4(Json::Value aJson) :
		x(aJson["X"].isNull() ? aJson["R"].asUInt() : aJson["X"].asUInt()),
		y(aJson["Y"].isNull() ? aJson["G"].asUInt() : aJson["Y"].asUInt()),
		z(aJson["Z"].isNull() ? aJson["B"].asUInt() : aJson["Z"].asUInt()),
		w(aJson["W"].isNull() ? aJson["A"].asUInt() : aJson["W"].asUInt())
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

	template <typename T>
	inline Json::Value Vector4<T>::ToJsonColor() const
	{
		Json::Value result;
		result["R"] = x;
		result["G"] = y;
		result["B"] = z;
		result["A"] = w;
		return result;
	}
}
