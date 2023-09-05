#pragma once
#include <Matrix4x4.hpp>
#include "Rendering/Buffers/ConstantBuffer.h"

const unsigned MAX_LIGHTS = 8;

struct PointlightData
{
	CommonUtilities::Matrix4x4f myView;
	CommonUtilities::Matrix4x4f myProjection;
	CommonUtilities::Vector3f myPosition;
	float myRadius;
	CommonUtilities::Vector3f myColor;
	float myIntensity;
};

struct SpotlightData
{
	CommonUtilities::Matrix4x4f myView;
	CommonUtilities::Matrix4x4f myProjection;
	CommonUtilities::Vector3f myPosition;
	float myRange;
	CommonUtilities::Vector3f myColor;
	float myIntensity;
	CommonUtilities::Vector3f myLightDirection;
	float myInnerAngle;
	float myOuterAngle;

	CommonUtilities::Vector3f padding;
};

struct LightBufferData
{
	CommonUtilities::Matrix4x4f myDirectionalView;
	CommonUtilities::Matrix4x4f myDirectionalProjection;
	CommonUtilities::Vector3f myInvertedDirection;
	float myDirectionallightIntensity;
	CommonUtilities::Vector3f myDirectionallightColor;
	float myAmbientlightIntensity;

	PointlightData myPointlights[MAX_LIGHTS];
	SpotlightData mySpotlights[MAX_LIGHTS];
};

typedef ConstantBuffer<LightBufferData> LightBuffer;