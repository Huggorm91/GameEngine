#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

struct FrameBufferData
{
	CommonUtilities::Matrix4x4f View{};
	CommonUtilities::Matrix4x4f Projection{};
	CommonUtilities::Vector3f Position{};
	float DeltaTime = 0.f;
	double TotalTime = 0.f;

	double padding;
};

typedef ConstantBuffer<FrameBufferData> FrameBuffer;