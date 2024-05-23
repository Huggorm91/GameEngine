#include "GraphicsEngine.pch.h"
#include "LitCmd_SetAmbientlight.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Texture.h"

LitCmd_SetAmbientlight::LitCmd_SetAmbientlight(const Texture* aCubemap, float anIntensity) : myCubemap(aCubemap), myIntensity(anIntensity)
{
}

LitCmd_SetAmbientlight::LitCmd_SetAmbientlight(const std::string& aCubemapPath, float anIntensity) : myCubemap(AssetManager::GetAsset<Texture*>(aCubemapPath)), myIntensity(anIntensity)
{
}

void LitCmd_SetAmbientlight::Execute(const int anIndex)
{
	UNREFERENCED_PARAMETER(anIndex);

	if (myCubemap)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, GetTextureSlots().DefaultCubeMapSlot, myCubemap);
	}	

	LightBuffer& buffer = GetLightBuffer();
	buffer.Data.AmbientlightIntensity = myIntensity;

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, GetLightBufferSlot(), buffer);
}