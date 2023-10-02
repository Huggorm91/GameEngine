#pragma once
#include <Vector4.hpp>

struct EmitterData
{
	float SpawnRate;
	float LifeTime;
	float GravityScale;
	CommonUtilities::Vector3f StartVelocity;
	CommonUtilities::Vector3f EndVelocity;
	CommonUtilities::Vector3f StartSize;
	CommonUtilities::Vector3f EndSize;
	CommonUtilities::Vector4f StartColor;
	CommonUtilities::Vector4f EndColor;
};