#include "GraphicsEngine.pch.h"
#include "LitCmd_SetShadowBias.h"

LitCmd_SetShadowBias::LitCmd_SetShadowBias(float aBias): myBias(aBias)
{
}

void LitCmd_SetShadowBias::Execute(const int anIndex)
{
	auto& buffer = GetLightBuffer();
	buffer.Data.ShadowBias = myBias;
	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
