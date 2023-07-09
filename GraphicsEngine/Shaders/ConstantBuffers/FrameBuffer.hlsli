#ifndef FRAMEBUFFER_HLSLI
#define FRAMEBUFFER_HLSLI
cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_View;
	float4x4 FB_Projection;
	float3 FB_CameraPosition;
	double FB_TotalTime;
	float FB_DeltaTime;
}
#endif // FRAMEBUFFER_HLSLI