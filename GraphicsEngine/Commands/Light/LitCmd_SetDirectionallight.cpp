#include "GraphicsEngine.pch.h"
#include "LitCmd_SetDirectionallight.h"

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(const DirectionallightComponent& aLight): LightCommand(), myInvertedLightDirection(aLight.GetInvertedLightDirection()), myColor(aLight.GetColor())
{
}

LitCmd_SetDirectionallight::LitCmd_SetDirectionallight(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor) : LightCommand(), myInvertedLightDirection(-aDirection), myColor(aColor)
{
}

void LitCmd_SetDirectionallight::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.InvertedDirection = myInvertedLightDirection;
	buffer.Data.DirectionalColor = { 1.f, 1.f, 1.f }; //myColor;
	buffer.Data.AmbientColor = myColor;
	buffer.Data.DiffuseColor = { 1.f, 0.9f, 0.7f };
	buffer.Data.SpecularColor = { 1.f, 1.f, 1.f };

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
