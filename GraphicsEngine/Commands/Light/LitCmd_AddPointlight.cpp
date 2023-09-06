#include "GraphicsEngine.pch.h"
#include "LitCmd_AddPointlight.h"

LitCmd_AddPointlight::LitCmd_AddPointlight(PointlightComponent& aLight) : LightCommand(Type::PointLight), myRadius(aLight.GetRadius()), myIntensity(aLight.GetIntensity()), myPosition(CommonUtilities::Vector4f{aLight.GetPosition(), 1.f} *aLight.GetTransform()), 
myColor(aLight.GetColor()), myShadowMap(aLight.GetShadowMap()), myCastsShadow(aLight.IsCastingShadows())
{
}

void LitCmd_AddPointlight::Execute(const int anIndex)
{
	LightBuffer& buffer = GetLightBuffer();

	PointlightData& data = buffer.Data.Pointlights[anIndex];
	data.Position = myPosition;
	data.Radius = myRadius;
	data.Color = myColor;
	data.Intensity = myIntensity;
	data.CastShadows = myCastsShadow;

	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::ClearDepthStencil(myShadowMap.get());
		GetPointlightShadowMap()[anIndex] = myShadowMap.get();
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);		
}

void LitCmd_AddPointlight::SetShadowMap(const int anIndex)
{
	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 120 + anIndex, myShadowMap.get());
	}
}
