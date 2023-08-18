#include "GraphicsEngine.pch.h"
#include "LitCmd_AddSpotlight.h"

LitCmd_AddSpotlight::LitCmd_AddSpotlight(const SpotlightComponent& aLight) : LightCommand(Type::SpotLight), myRange(aLight.GetRange()), myIntensity(aLight.GetIntensity()), myInnerAngle(aLight.GetInnerAngle()), myOuterAngle(aLight.GetOuterAngle()), myConeIntensityDifference(aLight.GetConeIntensityDifference()),
myPosition(CommonUtilities::Vector4f{aLight.GetPosition(), 1.f} *aLight.GetTransform()), myLightDirection(CommonUtilities::Vector4f{aLight.GetLightDirection(), 0.f} *aLight.GetTransform()), myColor(aLight.GetColor())
{
}

void LitCmd_AddSpotlight::Execute(const int anIndex)
{
	LightBuffer& buffer = GetLightBuffer();

	SpotlightData& data = buffer.Data.mySpotlights[anIndex];
	data.myPosition = myPosition;
	data.myRange = myRange;
	data.myColor = myColor;
	data.myIntensity = myIntensity;
	data.myLightDirection = myLightDirection;
	data.myInnerAngle = myInnerAngle;
	data.myOuterAngle = myOuterAngle;
	data.myConeIntensityDifference = myConeIntensityDifference;

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
