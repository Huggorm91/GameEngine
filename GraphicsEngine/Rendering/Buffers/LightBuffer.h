#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

struct PointlightData
{
	CommonUtilities::Vector3f myPosition;
	float myRadius;
	CommonUtilities::Vector3f myColor;
	float myIntensity;
};

struct SpotlightData
{
	CommonUtilities::Vector3f myPosition;
	float myRadius;
	CommonUtilities::Vector3f myColor;
	float myIntensity;
	CommonUtilities::Vector3f myLightDirection;
	float myInnerAngle;
	float myOuterAngle;
	float myConeIntensityDifference;

	CommonUtilities::Vector2f padding;
};

struct LightBufferData
{
	CommonUtilities::Vector3f InvertedDirection;
	float padding0;
	CommonUtilities::Vector3f DirectionalColor;
	float padding1;
	CommonUtilities::Vector3f AmbientColor;
	float padding2;
	CommonUtilities::Vector3f DiffuseColor;
	float padding3;
	CommonUtilities::Vector3f SpecularColor;
	float padding4;

	PointlightData Pointlights[8];
	SpotlightData Spotlights[8];
};

typedef ConstantBuffer<LightBufferData> LightBuffer;