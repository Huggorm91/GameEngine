#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

struct ObjectBufferData
{
	CommonUtilities::Matrix4x4f BoneTransforms[128]{};
	CommonUtilities::Matrix4x4f Transform{};
	CommonUtilities::Vector4f Color{};
	alignas(4) bool HasBones = false;
	CommonUtilities::Vector3f padding{};
};

typedef ConstantBuffer<ObjectBufferData> ObjectBuffer;