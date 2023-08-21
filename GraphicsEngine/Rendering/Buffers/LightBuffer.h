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
	float myRange;
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
	CommonUtilities::Vector3f myInvertedDirection;
	float myDirectionallightIntensity;
	CommonUtilities::Vector3f myDirectionallightColor;
	float myAmbientlightIntensity;

	PointlightData myPointlights[8];
	SpotlightData mySpotlights[8];
};

typedef ConstantBuffer<LightBufferData> LightBuffer;