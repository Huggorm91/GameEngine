#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

struct FrameBufferData
{
	CommonUtilities::Matrix4x4f View{};
	CommonUtilities::Matrix4x4f Projection{};
	CommonUtilities::Vector3f Position{};
	double TotalTime = 0.f;
	float DeltaTime = 0.f;
};

typedef ConstantBuffer<FrameBufferData> FrameBuffer;