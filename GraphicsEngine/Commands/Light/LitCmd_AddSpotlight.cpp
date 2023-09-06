#include "GraphicsEngine.pch.h"
#include "LitCmd_AddSpotlight.h"

LitCmd_AddSpotlight::LitCmd_AddSpotlight(SpotlightComponent& aLight) : LightCommand(Type::SpotLight), myRange(aLight.GetRange()), myIntensity(aLight.GetIntensity()), myInnerAngle(aLight.GetInnerAngle()), myOuterAngle(aLight.GetOuterAngle()),
myPosition(CommonUtilities::Vector4f{aLight.GetPosition(), 1.f} *aLight.GetTransform()), myLightDirection((CommonUtilities::Vector4f{aLight.GetLightDirection(), 0.f} *aLight.GetTransform()).GetNormalized()), myColor(aLight.GetColor()), 
myShadowMap(aLight.GetShadowMap()), myCastsShadow(aLight.IsCastingShadows())
{
}

void LitCmd_AddSpotlight::Execute(const int anIndex)
{
	LightBuffer& buffer = GetLightBuffer();

	SpotlightData& data = buffer.Data.Spotlights[anIndex];
	data.Position = myPosition;
	data.Range = myRange;
	data.Color = myColor;
	data.Intensity = myIntensity;
	data.LightDirection = myLightDirection;
	data.InnerAngle = myInnerAngle;
	data.OuterAngle = myOuterAngle;
	data.CastShadows = myCastsShadow;

	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::ClearDepthStencil(myShadowMap.get());
		GetSpotlightShadowMap()[anIndex] = myShadowMap.get();
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);	
}

void LitCmd_AddSpotlight::SetShadowMap(const int anIndex)
{
	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::ClearDepthStencil(myShadowMap.get());
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 87 + anIndex, myShadowMap.get());
	}
}
