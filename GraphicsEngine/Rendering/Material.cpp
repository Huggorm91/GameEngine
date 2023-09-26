#include "GraphicsEngine.pch.h"
#include "Material.h"
#include "../GraphicsEngine.h"
#include "../InterOp/Helpers.h"

#include "AssetManager/AssetManager.h"

#include <External/jsonCpp/json.h>

#ifndef _RETAIL
#include "AssetManager/DirectoryFunctions.h"

#include "ModelViewer/Core/ModelViewer.h"
#include "ModelViewer/Core/Commands/EditCmd_ChangeValue.h"

#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_stdlib.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"
#endif // !_RETAIL

Material::Material() : myVertexShader(nullptr), myPixelShader(nullptr), myTextures(), myBuffer(), myName(), myAlbedoTexture(nullptr), myNormalTexture(nullptr), myMaterialTexture(nullptr), myFXTexture(nullptr)
{
}

Material::Material(const Material& aMaterial) : myVertexShader(aMaterial.myVertexShader), myPixelShader(aMaterial.myPixelShader), myTextures(aMaterial.myTextures), myBuffer(aMaterial.myBuffer), myName(aMaterial.myName), myAlbedoTexture(aMaterial.myAlbedoTexture), myFXTexture(aMaterial.myFXTexture),
myNormalTexture(aMaterial.myNormalTexture), myMaterialTexture(aMaterial.myMaterialTexture)
{
	myBuffer.Initialize();
}

Material::Material(const Json::Value& aJsonValue) : myVertexShader(aJsonValue["VertexShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["VertexShader"].asString())),
myPixelShader(aJsonValue["PixelShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJsonValue["PixelShader"].asString())), myTextures(), myBuffer(), myName(aJsonValue["Name"].asString()),
myAlbedoTexture(aJsonValue["AlbedoTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["AlbedoTexture"].asString())),
myNormalTexture(aJsonValue["NormalTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["NormalTexture"].asString())),
myMaterialTexture(aJsonValue["MaterialTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["MaterialTexture"].asString())),
myFXTexture(aJsonValue["FXTexture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJsonValue["FXTexture"].asString()))
{
	myBuffer.Data.Shininess = aJsonValue["Shininess"].asFloat();
	myBuffer.Data.Metalness = aJsonValue["Metalness"].asFloat();
	myBuffer.Data.NormalStrength = aJsonValue["NormalStrength"].asFloat();
	myBuffer.Data.UVTiling = CommonUtilities::Vector2f(aJsonValue["UVTiling"]);
	myBuffer.Data.AlbedoColor = CommonUtilities::Vector4f(aJsonValue["AlbedoColor"]);
	myBuffer.Data.EmissionColor = CommonUtilities::Vector3f(aJsonValue["EmissionColor"]);
	myBuffer.Data.EmissionIntensity = aJsonValue["EmissionIntensity"].asFloat();

	for (unsigned i = 0; i < aJsonValue["Textures"].size(); i++)
	{
		const Json::Value& texture = aJsonValue["Textures"][i];
		myTextures.emplace_back(texture["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(texture["Texture"].asString()), texture["Stage"].asUInt(), texture["Slot"].asUInt());
	}
	myBuffer.Initialize();
}

Material::Material(const std::string& aName, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo, Texture* aNormal, Texture* aMaterial, Texture* aFX) : myVertexShader(aVertexShader), myPixelShader(aPixelShader), myTextures(),
myBuffer(), myName(aName), myAlbedoTexture(anAlbedo), myNormalTexture(aNormal), myMaterialTexture(aMaterial), myFXTexture(aFX)
{
	myBuffer.Initialize();
}

Material& Material::operator=(const Material& aMaterial)
{
	myVertexShader = aMaterial.myVertexShader;
	myPixelShader = aMaterial.myPixelShader;
	myTextures = aMaterial.myTextures;
	myName = aMaterial.myName;
	myAlbedoTexture = aMaterial.myAlbedoTexture;
	myNormalTexture = aMaterial.myNormalTexture;
	myMaterialTexture = aMaterial.myMaterialTexture;
	myFXTexture = aMaterial.myFXTexture;
	myBuffer = aMaterial.myBuffer;
	myBuffer.Initialize();
	return *this;
}

void Material::SetVertexShader(Shader* aShader)
{
	myVertexShader = aShader;
}

void Material::SetPixelShader(Shader* aShader)
{
	myPixelShader = aShader;
}

void Material::SetAlbedoTexture(Texture* aTexture)
{
	myAlbedoTexture = aTexture;
}

void Material::SetNormalTexture(Texture* aTexture)
{
	myNormalTexture = aTexture;
}

void Material::SetMaterialTexture(Texture* aTexture)
{
	myMaterialTexture = aTexture;
}

void Material::SetFXTexture(Texture* aTexture)
{
	myFXTexture = aTexture;
}

void Material::SetShininess(float aShininess)
{
	myBuffer.Data.Shininess = aShininess;
}

void Material::SetMetalness(float aMetalness)
{
	myBuffer.Data.Metalness = aMetalness;
}

void Material::SetNormalStrength(float aNormalStrength)
{
	myBuffer.Data.NormalStrength = aNormalStrength;
}

void Material::SetUVTiling(const CommonUtilities::Vector2f& aUVTiling)
{
	myBuffer.Data.UVTiling = aUVTiling;
}

void Material::SetAlbedoColor(const CommonUtilities::Vector4f& aColor)
{
	myBuffer.Data.AlbedoColor = aColor;
}

void Material::SetEmissionColor(const CommonUtilities::Vector3f& aColor)
{
	myBuffer.Data.EmissionColor = aColor;
}

void Material::SetEmissionIntensity(float anIntensity)
{
	myBuffer.Data.EmissionIntensity = anIntensity;
}

void Material::SetName(const std::string& aName)
{
	myName = aName;
}

const Shader* Material::GetVertexShader() const
{
	return myVertexShader;
}

const Shader* Material::GetPixelShader() const
{
	return myPixelShader;
}

const Texture* Material::GetAlbedoTexture() const
{
	return myAlbedoTexture;
}

const Texture* Material::GetNormalTexture() const
{
	return myNormalTexture;
}

const Texture* Material::GetMaterialTexture() const
{
	return myMaterialTexture;
}

const Texture* Material::GetFXTexture() const
{
	return myFXTexture;
}

float Material::GetShininess() const
{
	return myBuffer.Data.Shininess;
}

float Material::GetMetalness() const
{
	return myBuffer.Data.Metalness;
}

float Material::GetNormalStrength() const
{
	return myBuffer.Data.NormalStrength;
}

const CommonUtilities::Vector2f& Material::GetUVTiling() const
{
	return myBuffer.Data.UVTiling;
}

const CommonUtilities::Vector4f& Material::GetAlbedoColor() const
{
	return myBuffer.Data.AlbedoColor;
}

const CommonUtilities::Vector3f& Material::GetEmissionColor() const
{
	return myBuffer.Data.EmissionColor;
}

float Material::GetEmissionIntensity() const
{
	return myBuffer.Data.EmissionIntensity;
}

const std::string& Material::GetName() const
{
	return myName;
}

const MaterialBuffer& Material::GetBuffer() const
{
	return myBuffer;
}

Json::Value Material::ToJson() const
{
	Json::Value result;
	result["Name"] = myName;

	result["Textures"] = Json::arrayValue;
	for (unsigned i = 0; i < myTextures.size(); i++)
	{
		const TextureBinding& binding = myTextures[i];
		Json::Value texture;
		if (binding.texture)
		{
			texture["Texture"] = Helpers::string_cast<std::string>(binding.texture->GetName());
		}
		else
		{
			texture["Texture"] = Json::nullValue;
		}
		texture["Slot"] = binding.slot;
		texture["Stage"] = binding.stage;
		result["Textures"][i] = texture;
	}

	if (myVertexShader)
	{
		std::string path = Helpers::string_cast<std::string>(myVertexShader->GetName());
		size_t lastSlash = path.find_last_of('/') + 1;
		result["VertexShader"] = path.substr(lastSlash);

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["VertexShader"].setComment(comment, Json::commentAfterOnSameLine);
	}
	else
	{
		result["VertexShader"] = Json::nullValue;
	}
	if (myPixelShader)
	{
		std::string path = Helpers::string_cast<std::string>(myPixelShader->GetName());
		size_t lastSlash = path.find_last_of('/') + 1;
		result["PixelShader"] = path.substr(lastSlash);

		const std::string& comment = "// Only use 'ShaderName.cso' and not full path";
		result["PixelShader"].setComment(comment, Json::commentAfterOnSameLine);
	}
	else
	{
		result["PixelShader"] = Json::nullValue;
	}

	if (myAlbedoTexture)
	{
		result["AlbedoTexture"] = Helpers::string_cast<std::string>(myAlbedoTexture->GetName());
	}
	else
	{
		result["AlbedoTexture"] = Json::nullValue;
	}
	if (myNormalTexture)
	{
		result["NormalTexture"] = Helpers::string_cast<std::string>(myNormalTexture->GetName());
	}
	else
	{
		result["NormalTexture"] = Json::nullValue;
	}
	if (myMaterialTexture)
	{
		result["MaterialTexture"] = Helpers::string_cast<std::string>(myMaterialTexture->GetName());
	}
	else
	{
		result["MaterialTexture"] = Json::nullValue;
	}
	if (myFXTexture)
	{
		result["FXTexture"] = Helpers::string_cast<std::string>(myFXTexture->GetName());
	}
	else
	{
		result["FXTexture"] = Json::nullValue;
	}

	result["Shininess"] = myBuffer.Data.Shininess;
	result["Metalness"] = myBuffer.Data.Metalness;
	result["NormalStrength"] = myBuffer.Data.NormalStrength;
	result["UVTiling"] = static_cast<Json::Value>(myBuffer.Data.UVTiling);
	result["AlbedoColor"] = myBuffer.Data.AlbedoColor.ToJsonColor();
	result["EmissionColor"] = myBuffer.Data.EmissionColor.ToJsonColor();
	result["EmissionIntensity"] = myBuffer.Data.EmissionIntensity;
	return result;
}

void Material::CreateImguiComponents(const std::string&)
{
#ifndef _RETAIL
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Material"))
	{
		ImGui::Text(myName.c_str());
		if (myAlbedoTexture)
		{
			myAlbedoName = ToString(myAlbedoTexture->GetName());
		}
		if (myNormalTexture)
		{
			myNormalName = ToString(myNormalTexture->GetName());
		}
		if (myMaterialTexture)
		{
			myMaterialName = ToString(myMaterialTexture->GetName());
		}
		if (myFXTexture)
		{
			myFXName = ToString(myFXTexture->GetName());
		}

		// TODO: Fix Imgui representation for Shaders
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Textures"))
		{
			CreateTextureCombo(TextureSlot_Albedo);
			ImGui::SameLine();
			if (myAlbedoTexture)
			{
				ImGui::Image(myAlbedoTexture->GetSRV().Get(), ImVec2(75, 75));
			}
			else
			{
				ImGui::Image(GraphicsEngine::Get().GetDefaultMaterial().GetAlbedoTexture()->GetSRV().Get(), ImVec2(75, 75));
			}


			CreateTextureCombo(TextureSlot_Normal);
			ImGui::SameLine();
			if (myNormalTexture)
			{
				ImGui::Image(myNormalTexture->GetSRV().Get(), ImVec2(75, 75));
			}
			else
			{
				ImGui::Image(GraphicsEngine::Get().GetDefaultMaterial().GetNormalTexture()->GetSRV().Get(), ImVec2(75, 75));
			}


			CreateTextureCombo(TextureSlot_Material);
			ImGui::SameLine();
			if (myMaterialTexture)
			{
				ImGui::Image(myMaterialTexture->GetSRV().Get(), ImVec2(75, 75));
			}
			else
			{
				ImGui::Image(GraphicsEngine::Get().GetDefaultMaterial().GetMaterialTexture()->GetSRV().Get(), ImVec2(75, 75));
			}


			CreateTextureCombo(TextureSlot_FX);
			ImGui::SameLine();
			if (myFXTexture)
			{
				ImGui::Image(myFXTexture->GetSRV().Get(), ImVec2(75, 75));
			}
			else
			{
				ImGui::Image(GraphicsEngine::Get().GetDefaultMaterial().GetFXTexture()->GetSRV().Get(), ImVec2(75, 75));
			}

			if (!myTextures.empty())
			{
				ImGui::SeparatorText("Extra Textures");
				for (auto& binding : myTextures)
				{
					ImGui::Text(("Slot " + std::to_string(binding.slot) + ": ").c_str());
					ImGui::SameLine();
					ImGui::Image(binding.texture->GetSRV().Get(), ImVec2(75, 75));
				}
			}
			ImGui::TreePop();
		}

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Buffer"))
		{
			auto& buffer = myBuffer.Data;

			auto albedo = buffer.AlbedoColor;
			if (ImGui::ColorEdit4("Albedo Color", &albedo.x))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<CommonUtilities::Vector4f>>(buffer.AlbedoColor, albedo));
			}

			auto emission = buffer.EmissionColor;
			if (ImGui::ColorEdit3("Emission Color", &emission.x))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<CommonUtilities::Vector3f>>(buffer.EmissionColor, emission));
			}

			auto intensity = buffer.EmissionIntensity;
			if (ImGui::DragFloat("Emission Intensity", &intensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<float>>(buffer.EmissionIntensity, intensity));
			}

			auto uv = buffer.UVTiling;
			if (ImGui::DragFloat2("UV Tiling", &uv.x, 0.1f))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<CommonUtilities::Vector2f>>(buffer.UVTiling, uv));
			}

			auto normal = buffer.NormalStrength;
			if (ImGui::DragFloat("Normal Strength", &normal, 0.01f))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<float>>(buffer.NormalStrength, normal));
			}

			// Not implemented: Blinn Phong variables

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif // !_RETAIL
}

#ifndef _RETAIL
void Material::CreateTextureCombo(eTextureSlot aSlot)
{
	Texture** texture = nullptr;
	std::string* name = nullptr;

	switch (aSlot)
	{
	case TextureSlot_Albedo:
	{
		texture = &myAlbedoTexture;
		name = &myAlbedoName;
		break;
	}
	case TextureSlot_Normal:
	{
		texture = &myNormalTexture;
		name = &myNormalName;
		break;
	}
	case TextureSlot_Material:
	{
		texture = &myMaterialTexture;
		name = &myMaterialName;
		break;
	}
	case TextureSlot_FX:
	{
		texture = &myFXTexture;
		name = &myFXName;
		break;
	}
	default:
		return;
	}

	ImGui::PushID(aSlot);
	if (ImGui::BeginCombo("", name->c_str(), ImGuiComboFlags_HeightLarge))
	{
		for (auto& path : AssetManager::GetAvailableTextures())
		{
			Texture* current = AssetManager::GetAsset<Texture*>(path);
			const bool isSelected = *texture == current;
			if (ImGui::Selectable(ToString(current->GetName()).c_str(), isSelected))
			{
				*texture = current;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			constexpr int imageSize = 30;
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - imageSize);
			ImGui::Image(current->GetSRV().Get(), ImVec2(imageSize, imageSize));
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}
#endif // !_RETAIL

void Material::AddTexture(Texture* aTexture, unsigned aPipelineStage, unsigned aSlot)
{
	myTextures.emplace_back(aTexture, aPipelineStage, aSlot);
}
