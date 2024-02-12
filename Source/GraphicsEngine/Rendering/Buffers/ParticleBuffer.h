#pragma once
#include "Math/Matrix4x4.hpp"
#include "ConstantBuffer.h"

struct ParticleBufferData
{
    float MaxLifeTime{};
    Crimson::Vector3f StartSize;
    Crimson::Vector3f EndSize;
    Crimson::Vector4f StartColor;
    Crimson::Vector4f EndColor;
	Crimson::Matrix4x4f Transform;

	float padding{};
};

typedef ConstantBuffer<ParticleBufferData> ParticleBuffer;