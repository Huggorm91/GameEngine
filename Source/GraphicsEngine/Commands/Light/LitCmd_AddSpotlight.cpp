#include "GraphicsEngine.pch.h"
#include "LitCmd_AddSpotlight.h"

LitCmd_AddSpotlight::LitCmd_AddSpotlight(SpotlightComponent& aLight) : LightCommand(Type::SpotLight), myRange(aLight.GetRange()), myIntensity(aLight.GetIntensity()), myInnerAngle(aLight.GetInnerAngle()), myOuterAngle(aLight.GetOuterAngle()),
myPosition(Crimson::Vector4f{aLight.GetPosition(), 1.f} * aLight.GetTransform()), myLightDirection((Crimson::Vector4f{aLight.GetLightDirection(), 0.f} * aLight.GetTransform()).GetNormalized()), myColor(aLight.GetColor()), 
myShadowMap(aLight.GetShadowMap()), myCastsShadow(aLight.IsCastingShadows())
{
	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::ClearDepthStencil(myShadowMap.get());
	}
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
		GetSpotlightShadowMap()[anIndex] = myShadowMap.get();
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_PIXEL_SHADER, GetLightBufferSlot(), buffer);
}

Texture* LitCmd_AddSpotlight::GetShadowMap()
{
	return myShadowMap.get();
}

void LitCmd_AddSpotlight::SetShadowMap(const int anIndex)
{
	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, GetTextureSlots().SpotShadowMapSlot + anIndex, myShadowMap.get());
	}
}

bool LitCmd_AddSpotlight::CastsShadow() const
{
	return myCastsShadow && myIntensity > 0.f;
}
