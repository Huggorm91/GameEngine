#pragma once
#include "Math/Matrix4x4.hpp"
#include "ConstantBuffer.h"

struct FrameBufferData
{
	Crimson::Matrix4x4f View{};
	Crimson::Matrix4x4f Projection{};
	Crimson::Vector3f CameraPosition{};
	float DeltaTime = 0.f;
	double TotalTime = 0.f;
	Crimson::Vector2f ScreenSize{};

#ifndef _RETAIL
	int DebugMode;
	int LightMode;

	Crimson::Vector2f padding;
#endif // _DEBUG
};

typedef ConstantBuffer<FrameBufferData> FrameBuffer;