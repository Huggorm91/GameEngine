#pragma once
#include "Math/Vector4.hpp"

struct EmitterData
{
	float SpawnRate;
	float LifeTime;
	float GravityScale;
	Crimson::Vector3f StartVelocity;
	Crimson::Vector3f EndVelocity;
	Crimson::Vector3f StartSize;
	Crimson::Vector3f EndSize;
	Crimson::Vector4f StartColor;
	Crimson::Vector4f EndColor;
};