#include "GraphicsEngine.pch.h"
#include "LitCmd_ResetLightBuffer.h"

void LitCmd_ResetLightBuffer::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.myInvertedDirection = CommonUtilities::Vector3f::Null;
	buffer.Data.AmbientColor = CommonUtilities::Vector3f::Null;
	buffer.Data.DiffuseColor = CommonUtilities::Vector3f::Null;
	buffer.Data.SpecularColor = CommonUtilities::Vector3f::Null;

	PointlightData pointlight{};
	SpotlightData spotlight{};

	for (int i = 0; i < 8; i++)
	{
		buffer.Data.myPointlights[i] = pointlight;
		buffer.Data.mySpotlights[i] = spotlight;
	}

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
