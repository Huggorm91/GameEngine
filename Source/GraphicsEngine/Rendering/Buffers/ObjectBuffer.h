#pragma once
#include "CrimsonUtilities/Math/Matrix4x4.hpp"
#include "ConstantBuffer.h"
#include "GraphicsEngine/GraphicsEngineDefines.h"

struct ObjectBufferData
{
	Crimson::Matrix4x4f BoneTransforms[MAX_BONE_COUNT]{};
	Crimson::Matrix4x4f Transform{};
	Crimson::Matrix4x4f TransformInverse{};
	Crimson::Vector4f Color{};
	unsigned ObjectID{};
	alignas(4) bool HasBones = false;
	Crimson::Vector2f padding{};
};

typedef ConstantBuffer<ObjectBufferData> ObjectBuffer;