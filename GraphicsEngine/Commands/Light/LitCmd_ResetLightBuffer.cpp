#include "GraphicsEngine.pch.h"
#include "LitCmd_ResetLightBuffer.h"

void LitCmd_ResetLightBuffer::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.InvertedDirection = CommonUtilities::Vector3f::Null;
	buffer.Data.DirectionallightColor = CommonUtilities::Vector3f::Null;
	buffer.Data.DirectionallightIntensity = 0.f;
	buffer.Data.CastDirectionalShadows = false;

	PointlightData pointlight{};
	SpotlightData spotlight{};

	for (int i = 0; i < 8; i++)
	{
		buffer.Data.Pointlights[i] = pointlight;
		buffer.Data.Spotlights[i] = spotlight;
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}