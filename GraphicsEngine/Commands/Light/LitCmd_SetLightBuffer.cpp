#include "GraphicsEngine.pch.h"
#include "LitCmd_SetLightBuffer.h"

LitCmd_SetLightBuffer::LitCmd_SetLightBuffer(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& anAmbientColor, const CommonUtilities::Vector3f& aDiffuseColor, const CommonUtilities::Vector3f& aSpecularColor):
	myInvertedDirection(-aDirection.GetNormalized()), myAmbientColor(anAmbientColor), myDiffuseColor(aDiffuseColor), mySpecularColor(aSpecularColor)
{
}

void LitCmd_SetLightBuffer::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.myInvertedDirection = myInvertedDirection;
	buffer.Data.AmbientColor = myAmbientColor;
	buffer.Data.DiffuseColor = myDiffuseColor;
	buffer.Data.SpecularColor = mySpecularColor;

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, buffer);
}
