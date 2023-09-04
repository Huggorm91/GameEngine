#include "GraphicsEngine.pch.h"
#include "LitCmd_AddSpotlight.h"

LitCmd_AddSpotlight::LitCmd_AddSpotlight(SpotlightComponent& aLight) : LightCommand(Type::SpotLight), myRange(aLight.GetRange()), myIntensity(aLight.GetIntensity()), myInnerAngle(aLight.GetInnerAngle()), myOuterAngle(aLight.GetOuterAngle()),
myPosition(CommonUtilities::Vector4f{aLight.GetPosition(), 1.f} *aLight.GetTransform()), myLightDirection((CommonUtilities::Vector4f{aLight.GetLightDirection(), 0.f} *aLight.GetTransform()).GetNormalized()), myColor(aLight.GetColor()), myShadowMap()
{
	myShadowMap = aLight.GetShadowMap();
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

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);	
}

void LitCmd_AddSpotlight::SetShadowMap(const int anIndex)
{
	if (myShadowMap != nullptr)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 87 + anIndex, myShadowMap.get());
	}
}
