#pragma once
#include "Math/Matrix4x4.hpp"
#include "ConstantBuffer.h"

const unsigned MAX_LIGHTS = 8;	// Update registers for ShadowMaps if MAX_LIGHTS is changed

struct PointlightData
{
	Crimson::Matrix4x4f View;
	Crimson::Matrix4x4f Projection;
	Crimson::Vector3f Position;
	float Radius;
	Crimson::Vector3f Color;
	float Intensity;
	alignas(4) bool CastShadows;

	Crimson::Vector3f padding;
};

struct SpotlightData
{
	Crimson::Matrix4x4f View;
	Crimson::Matrix4x4f Projection;
	Crimson::Vector3f Position;
	float Range;
	Crimson::Vector3f Color;
	float Intensity;
	Crimson::Vector3f LightDirection;
	float InnerAngle;
	float OuterAngle;
	alignas(4) bool CastShadows;

	Crimson::Vector2f padding;
};

struct LightBufferData
{
	Crimson::Matrix4x4f DirectionalView;
	Crimson::Matrix4x4f DirectionalProjection;
	Crimson::Vector3f InvertedDirection;
	float DirectionallightIntensity;
	Crimson::Vector3f DirectionallightColor;
	float AmbientlightIntensity;
	float ShadowBias;
	alignas(4) bool CastDirectionalShadows;
	int LB_ToneMapMode;

	float paddinglb;

	PointlightData Pointlights[MAX_LIGHTS];
	SpotlightData Spotlights[MAX_LIGHTS];
};

typedef ConstantBuffer<LightBufferData> LightBuffer;