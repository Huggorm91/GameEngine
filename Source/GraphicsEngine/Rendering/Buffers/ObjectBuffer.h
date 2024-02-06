#pragma once
#include "Math/Matrix4x4.hpp"
#include "Rendering/Buffers/ConstantBuffer.h"

struct ObjectBufferData
{
	Crimson::Matrix4x4f BoneTransforms[128]{};
	Crimson::Matrix4x4f Transform{};
	Crimson::Matrix4x4f TransformInverse{};
	Crimson::Vector4f Color{};
	unsigned ObjectID{};
	alignas(4) bool HasBones = false;
	Crimson::Vector2f padding{};
};

typedef ConstantBuffer<ObjectBufferData> ObjectBuffer;