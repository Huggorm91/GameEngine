#ifndef FRAMEBUFFER_HLSLI
#define FRAMEBUFFER_HLSLI
cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_View;
	float4x4 FB_Projection;
	float3 FB_CameraPosition;
	float FB_DeltaTime;
	double FB_TotalTime;
	
#ifdef _DEBUG
	int FB_DebugMode;
	float paddingfb;
#else
    double paddingfb;	
#endif
}
#endif // FRAMEBUFFER_HLSLI