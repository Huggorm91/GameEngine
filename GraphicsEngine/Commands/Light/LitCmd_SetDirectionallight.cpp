#include "GraphicsEngine.pch.h"
#include "LitCmd_SetDirectionallight.h"

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(DirectionallightComponent& aLight) : LightCommand(Type::Directional), myInvertedLightDirection(aLight.GetInvertedLightDirection()), myColor(aLight.GetColor()), myIntensity(aLight.GetIntensity()), myShadowMap(aLight.GetShadowMap()), myCastsShadow(aLight.IsCastingShadows())
{
}

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor, float anIntensity) : LightCommand(), myInvertedLightDirection(-aDirection), myColor(aColor), myIntensity(anIntensity), myShadowMap(nullptr), myCastsShadow(false)
{
}

void LitCmd_SetDirectionallight::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.InvertedDirection = myInvertedLightDirection;
	buffer.Data.DirectionallightColor = myColor;
	buffer.Data.DirectionallightIntensity = myIntensity;
	buffer.Data.CastDirectionalShadows = myCastsShadow;

	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::ClearDepthStencil(myShadowMap.get());
		GetDirectionalShadowMap() = myShadowMap.get();
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}

void LitCmd_SetDirectionallight::SetShadowMap(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);
	if (myCastsShadow && myShadowMap != nullptr)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 95, myShadowMap.get());
	}
}
