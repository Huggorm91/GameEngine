#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

struct FrameBufferData
{
	CommonUtilities::Matrix4x4f View{};
	CommonUtilities::Matrix4x4f Projection{};
	CommonUtilities::Vector3f CameraPosition{};
	float DeltaTime = 0.f;
	double TotalTime = 0.f;
	CommonUtilities::Vector2f ScreenSize{};

#ifndef _RETAIL
	int DebugMode;
	int LightMode;

	CommonUtilities::Vector2f padding;
#endif // _DEBUG
};

typedef ConstantBuffer<FrameBufferData> FrameBuffer;