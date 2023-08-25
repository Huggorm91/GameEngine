#include "GraphicsEngine.pch.h"
#include "LitCmd_SetDirectionallight.h"

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(const DirectionallightComponent& aLight): LightCommand(), myInvertedLightDirection(aLight.GetInvertedLightDirection()), myColor(aLight.GetColor()), myIntensity(aLight.GetIntensity()), myShadowMap(aLight.GetShadowMap())
{
}

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor, float anIntensity) : LightCommand(), myInvertedLightDirection(-aDirection), myColor(aColor), myIntensity(anIntensity), myShadowMap(nullptr)
{
}

void LitCmd_SetDirectionallight::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.myInvertedDirection = myInvertedLightDirection;
	buffer.Data.myDirectionallightColor = myColor;
	buffer.Data.myDirectionallightIntensity = myIntensity;

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
