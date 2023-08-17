#include "GraphicsEngine.pch.h"
#include "LitCmd_AddPointlight.h"

LitCmd_AddPointlight::LitCmd_AddPointlight(const PointlightComponent& aLight) : LightCommand(Type::PointLight), myRadius(aLight.GetRadius()), myIntensity(aLight.GetIntensity()), myPosition(CommonUtilities::Vector4f{aLight.GetPosition(), 1.f} *aLight.GetTransform()), myColor(aLight.GetColor())
{
}

void LitCmd_AddPointlight::Execute(const int anIndex)
{
	LightBuffer& buffer = GetLightBuffer();

	PointlightData& data = buffer.Data.myPointlights[anIndex];
	data.myPosition = myPosition;
	data.myRadius = myRadius;
	data.myColor = myColor;
	data.myIntensity = myIntensity;

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
