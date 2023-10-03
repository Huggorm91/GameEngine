#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

const unsigned MAX_LIGHTS = 8;	// Update registers for ShadowMaps if MAX_LIGHTS is changed

struct PointlightData
{
	CommonUtilities::Matrix4x4f View;
	CommonUtilities::Matrix4x4f Projection;
	CommonUtilities::Vector3f Position;
	float Radius;
	CommonUtilities::Vector3f Color;
	float Intensity;
	alignas(4) bool CastShadows;

	CommonUtilities::Vector3f padding;
};

struct SpotlightData
{
	CommonUtilities::Matrix4x4f View;
	CommonUtilities::Matrix4x4f Projection;
	CommonUtilities::Vector3f Position;
	float Range;
	CommonUtilities::Vector3f Color;
	float Intensity;
	CommonUtilities::Vector3f LightDirection;
	float InnerAngle;
	float OuterAngle;
	alignas(4) bool CastShadows;

	CommonUtilities::Vector2f padding;
};

struct LightBufferData
{
	CommonUtilities::Matrix4x4f DirectionalView;
	CommonUtilities::Matrix4x4f DirectionalProjection;
	CommonUtilities::Vector3f InvertedDirection;
	float DirectionallightIntensity;
	CommonUtilities::Vector3f DirectionallightColor;
	float AmbientlightIntensity;
	float ShadowBias;
	alignas(4) bool CastDirectionalShadows;
	int LB_ToneMapMode;

	float paddinglb;

	PointlightData Pointlights[MAX_LIGHTS];
	SpotlightData Spotlights[MAX_LIGHTS];
};

typedef ConstantBuffer<LightBufferData> LightBuffer;