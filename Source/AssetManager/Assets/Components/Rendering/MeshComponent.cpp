#include "AssetManager.pch.h"
#include "MeshComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"
#include "GraphicsEngine/Commands/GfxCmd_UpdateWorldBounds.h"

#ifndef _RETAIL
#include "AssetManager.h"
#include "ModelViewer/Core/ModelViewer.h"
#include "ModelViewer/Core/Commands/EditCmd_ChangeValue.h"
#endif // !_RETAIL

MeshComponent::MeshComponent() : Component(ComponentType::Mesh), myTransform(), myColor{ 0.f, 0.f, 0.f, 1.f }, myElements(), myBoxSphereBounds(), myRenderShadow(true), myName(), myPath(nullptr), myTransformMatrix(), myIsDeferred(true)
#ifndef _RETAIL
, myLerpValue(), myLerpColor1{ 0.f, 0.f, 0.f, 1.f }, myLerpColor2{ 0.f, 0.f, 0.f, 1.f }
#endif // !_RETAIL
{
}

MeshComponent::MeshComponent(ComponentType aType) : Component(aType), myTransform(), myColor{ 0.f, 0.f, 0.f, 1.f }, myElements(), myBoxSphereBounds(), myRenderShadow(true), myName(), myPath(nullptr), myTransformMatrix(), myIsDeferred(true)
#ifndef _RETAIL
, myLerpValue(), myLerpColor1{ 0.f, 0.f, 0.f, 1.f }, myLerpColor2{ 0.f, 0.f, 0.f, 1.f }
#endif // !_RETAIL
{
}

MeshComponent::MeshComponent(const TGA::FBX::Mesh& aMesh, const std::vector<MeshElement>& anElementList, const std::string* aPath, ComponentType aType) : Component(aType), myElements(anElementList), myName(aMesh.Name), myPath(aPath), myColor{ 0.f, 0.f, 0.f, 1.f },
myBoxSphereBounds(aMesh.BoxSphereBounds), myTransform(), myRenderShadow(true), myTransformMatrix(), myIsDeferred(true)
#ifndef _RETAIL
, myLerpValue(), myLerpColor1{ 0.f, 0.f, 0.f, 1.f }, myLerpColor2{ 0.f, 0.f, 0.f, 1.f }
#endif // !_RETAIL
{
	if (aMesh.BoxBounds.IsValid && aMesh.BoxSphereBounds.Radius == 0.f)
	{
		myBoxSphereBounds = aMesh.BoxBounds;
	}
}

MeshComponent::MeshComponent(const MeshComponent& aMeshComponent) : Component(aMeshComponent), myTransform(aMeshComponent.myTransform), myColor(aMeshComponent.myColor), myElements(aMeshComponent.myElements), myBoxSphereBounds(aMeshComponent.myBoxSphereBounds),
myName(aMeshComponent.myName), myPath(aMeshComponent.myPath), myRenderShadow(aMeshComponent.myRenderShadow), myTransformMatrix(aMeshComponent.myTransformMatrix), myIsDeferred(aMeshComponent.myIsDeferred)
#ifndef _RETAIL
, myLerpValue(aMeshComponent.myLerpValue), myLerpColor1(aMeshComponent.myLerpColor1), myLerpColor2(aMeshComponent.myLerpColor2)
#endif // !_RETAIL
{
	if (myParent)
	{
		myTransform.SetParent(GetParentTransform());
	}
}

MeshComponent::MeshComponent(MeshComponent&& aMeshComponent) noexcept : Component(aMeshComponent), myTransform(aMeshComponent.myTransform), myColor(aMeshComponent.myColor), myElements(aMeshComponent.myElements), myBoxSphereBounds(aMeshComponent.myBoxSphereBounds),
myName(aMeshComponent.myName), myPath(aMeshComponent.myPath), myRenderShadow(aMeshComponent.myRenderShadow), myTransformMatrix(aMeshComponent.myTransformMatrix), myIsDeferred(aMeshComponent.myIsDeferred)
#ifndef _RETAIL
, myLerpValue(aMeshComponent.myLerpValue), myLerpColor1(aMeshComponent.myLerpColor1), myLerpColor2(aMeshComponent.myLerpColor2)
#endif // !_RETAIL
{
	if (myParent)
	{
		myTransform.SetParent(GetParentTransform());
	}
}

MeshComponent& MeshComponent::operator=(const MeshComponent& aMeshComponent)
{
	Component::operator=(aMeshComponent);
	myTransform = aMeshComponent.myTransform;
	myColor = aMeshComponent.myColor;
	myElements = aMeshComponent.myElements;
	myBoxSphereBounds = aMeshComponent.myBoxSphereBounds;
	myName = aMeshComponent.myName;
	myPath = aMeshComponent.myPath;
	myRenderShadow = aMeshComponent.myRenderShadow;
	myTransformMatrix = aMeshComponent.myTransformMatrix;
	myIsDeferred = aMeshComponent.myIsDeferred;
#ifndef _RETAIL
	myLerpValue = aMeshComponent.myLerpValue;
	myLerpColor1 = aMeshComponent.myLerpColor1;
	myLerpColor2 = aMeshComponent.myLerpColor2;
#endif // !_RETAIL
	if (myParent)
	{
		myTransform.SetParent(GetParentTransform());
	}

	return *this;
}

MeshComponent& MeshComponent::operator=(MeshComponent&& aMeshComponent) noexcept
{
	Component::operator=(std::move(aMeshComponent));
	myTransform = aMeshComponent.myTransform;
	myColor = aMeshComponent.myColor;
	myElements = aMeshComponent.myElements;
	myBoxSphereBounds = aMeshComponent.myBoxSphereBounds;
	myName = aMeshComponent.myName;
	myPath = aMeshComponent.myPath;
	myRenderShadow = aMeshComponent.myRenderShadow;
	myTransformMatrix = aMeshComponent.myTransformMatrix;
	myIsDeferred = aMeshComponent.myIsDeferred;
#ifndef _RETAIL
	myLerpValue = aMeshComponent.myLerpValue;
	myLerpColor1 = aMeshComponent.myLerpColor1;
	myLerpColor2 = aMeshComponent.myLerpColor2;
#endif // !_RETAIL
	if (myParent)
	{
		myTransform.SetParent(GetParentTransform());
	}

	return *this;
}

void MeshComponent::Update()
{
	Render();
}

void MeshComponent::Render()
{
	if (!myIsActive)
	{
		return;
	}

	if (myRenderShadow)
	{
		GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMeshShadow>(*this));
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMesh>(*this, myIsDeferred));
}

void MeshComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	myTransform.SetParent(GetParentTransform());
	TransformHasChanged();
}

void MeshComponent::Init(const Json::Value& aJson)
{
	Component::Init(aJson);
	myRenderShadow = aJson["RenderShadow"].asBool();
	myColor = Crimson::Vector4f(aJson["Color"]);
	myTransform = aJson["Transform"];
	myTransform.SetParent(GetParentTransform());
	myIsDeferred = aJson["IsDeferred"].asBool();
	for (auto& element : aJson["Elements"])
	{
		myElements[element["Index"].asInt()].myMaterial = element["Material"];
	}

#ifndef _RETAIL
	myLerpValue = aJson["LerpValue"].asFloat();
	myLerpColor1 = Crimson::Vector4f(aJson["LerpColor1"]);
	myLerpColor2 = Crimson::Vector4f(aJson["LerpColor2"]);
#endif // !_RETAIL
}

void MeshComponent::Init(const std::vector<MeshElement>& anElementList, const std::string& aName, const std::string* aPath)
{
	myElements = anElementList;
	myName = aName;
	myPath = aPath;

	Crimson::Vector3f min;
	Crimson::Vector3f max;

	for (auto& element : myElements)
	{
		const Crimson::Vector3f& elementMin = element.myData->myBoxSphereBounds.GetMin();
		const Crimson::Vector3f& elementMax = element.myData->myBoxSphereBounds.GetMax();
		if (elementMin.x < min.x)
		{
			min.x = elementMin.x;
		}
		if (elementMin.y < min.y)
		{
			min.y = elementMin.y;
		}
		if (elementMin.z < min.z)
		{
			min.z = elementMin.z;
		}

		if (elementMax.x > max.x)
		{
			max.x = elementMax.x;
		}
		if (elementMax.y > max.y)
		{
			max.y = elementMax.y;
		}
		if (elementMax.z > max.z)
		{
			max.z = elementMax.z;
		}
	}

	Crimson::Vector3f center = (min + max) * 0.5f;
	Crimson::Vector3f size = max - min;
	myBoxSphereBounds.Init(center, size);
}

void MeshComponent::SetOffsetPosition(const Crimson::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void MeshComponent::SetOffsetRotation(const Crimson::Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void MeshComponent::SetOffsetScale(const Crimson::Vector3f& aScale)
{
	myTransform.SetScale(aScale);
}

const Crimson::Matrix4x4f& MeshComponent::GetTransform() const
{
	if (myTransform.HasChanged())
	{
		TransformHasChanged();
	}
	return myTransform.GetTransformMatrix();
}

const Crimson::Vector4f& MeshComponent::GetWorldPosition() const
{
	return myTransform.GetWorldPosition();
}

const std::vector<MeshElement>& MeshComponent::GetElements() const
{
	return myElements;
}

std::vector<MeshElement>& MeshComponent::GetElements()
{
	return myElements;
}

void MeshComponent::SetAlbedoTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetAlbedoTexture(aTexture);
	}
}

void MeshComponent::SetNormalTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetNormalTexture(aTexture);
	}
}

void MeshComponent::SetMaterialTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetMaterialTexture(aTexture);
	}
}

void MeshComponent::SetFXTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetFXTexture(aTexture);
	}
}

void MeshComponent::SetColor(const Crimson::Vector4f& aColor)
{
	myColor = aColor;
}

const Crimson::Vector4f& MeshComponent::GetColor() const
{
	return myColor;
}

void MeshComponent::SetRenderShadow(bool aState)
{
	myRenderShadow = aState;
}

void MeshComponent::ToogleRenderShadow()
{
	myRenderShadow = !myRenderShadow;
}

bool MeshComponent::IsRenderingShadow() const
{
	return myRenderShadow;
}

void MeshComponent::SetIsDeferred(bool aState)
{
	myIsDeferred = aState;
}

bool MeshComponent::IsDeferred() const
{
	return myIsDeferred;
}

const std::string& MeshComponent::GetName() const
{
	return myName;
}

void MeshComponent::TransformHasChanged() const
{
	const_cast<Transform&>(myTransform).SetHasChanged(true);
	const auto& transform = myTransform.GetTransformMatrix();
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_UpdateWorldBounds>(transform * Crimson::Vector4f(myBoxSphereBounds.GetMin(), 1.f), transform * Crimson::Vector4f(myBoxSphereBounds.GetMax(), 1.f)));
}

void MeshComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	ImGui::Checkbox("Render Shadow", &myRenderShadow);
	if (ColorManager::CreateImGuiComponents(myLerpColor1, myLerpName1, "Color1"))
	{
		myColor = Crimson::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	ImGui::SameLine();
	ImGui::ColorEdit4("   ", &myLerpColor1.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

	if (ColorManager::CreateImGuiComponents(myLerpColor2, myLerpName2, "Color2"))
	{
		myColor = Crimson::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	ImGui::SameLine();
	ImGui::ColorEdit4("    ", &myLerpColor2.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

	if (ImGui::SliderFloat("Color", &myLerpValue, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
	{
		myColor = Crimson::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	ImGui::SameLine();
	ImGui::ColorEdit4("  ", &myColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

	ImGui::Checkbox("Is Deferred Rendered", &myIsDeferred);
	
	int i = 0;
	for (auto& element : myElements)
	{
		ImGui::PushID(i++);
		CreateMaterialImGui(element.myMaterial);
		ImGui::PopID();
	}

	myTransform.CreateImGuiComponents(aWindowName);
}

Json::Value MeshComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["RenderShadow"] = myRenderShadow;
	result["IsDeferred"] = myIsDeferred;
	result["Color"] = myColor.ToJsonColor();
	result["Transform"] = myTransform.ToJson();
	if (myPath)
	{
		result["Path"] = *myPath;
	}
	result["Elements"] = Json::arrayValue;
	for (int i = 0; i < myElements.size(); i++)
	{
		auto& element = result["Elements"][i];
		element["Index"] = i;
		element["Material"] = myElements[i].myMaterial.ToJson();
	}

#ifndef _RETAIL
	result["LerpValue"] = myLerpValue;
	result["LerpColor1"] = myLerpColor1.ToJsonColor();
	result["LerpColor2"] = myLerpColor2.ToJsonColor();
#endif // !_RETAIL

	return result;
}

inline std::string MeshComponent::ToString() const
{
	return "Mesh";
}

const MeshComponent* MeshComponent::GetTypePointer() const
{
	return this;
}

#ifndef _RETAIL
void MeshComponent::CreateMaterialImGui(Material& aMaterial)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
	if (ImGui::TreeNode("Material"))
	{
		ImGui::Text(myName.c_str());

		// TODO: Fix Imgui representation for Shaders
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Textures"))
		{
			Texture albedo;
			CreateTextureCombo(aMaterial.myAlbedoTexture, TextureSlot_Albedo);
			ImGui::SameLine();
			CreateTextureImage(aMaterial.myAlbedoTexture);

			CreateTextureCombo(aMaterial.myNormalTexture, TextureSlot_Normal);
			ImGui::SameLine();
			CreateTextureImage(aMaterial.myNormalTexture);

			CreateTextureCombo(aMaterial.myMaterialTexture, TextureSlot_Material);
			ImGui::SameLine();
			CreateTextureImage(aMaterial.myMaterialTexture);

			CreateTextureCombo(aMaterial.myFXTexture, TextureSlot_FX);
			ImGui::SameLine();
			CreateTextureImage(aMaterial.myFXTexture);

			if (!aMaterial.myTextures.empty())
			{
				ImGui::SeparatorText("Extra Textures");
				for (auto& binding : aMaterial.myTextures)
				{
					ImGui::Text(("Slot " + std::to_string(binding.slot) + ": ").c_str());
					ImGui::SameLine();
					CreateTextureImage(binding.texture);
				}
			}
			ImGui::TreePop();
		}

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Buffer"))
		{
			auto& buffer = aMaterial.myBuffer.Data;

			auto albedo = buffer.AlbedoColor;
			if (ImGui::ColorEdit4("Albedo Color", &albedo.x))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<Crimson::Vector4f>>(buffer.AlbedoColor, albedo));
			}

			auto emission = buffer.EmissionColor;
			if (ImGui::ColorEdit4("Emission Color", &emission.x))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<Crimson::Vector4f>>(buffer.EmissionColor, emission));
			}
			ImGui::SameLine();
			ImGui::TextDisabled("(?)");
			if (ImGui::BeginItemTooltip())
			{
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Alpha controls how much of the Albedo texture is used for Emission.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			auto intensity = buffer.EmissionIntensity;
			if (ImGui::DragFloat("Emission Intensity", &intensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<float>>(buffer.EmissionIntensity, intensity));
			}

			auto uv = buffer.UVTiling;
			if (ImGui::DragFloat2("UV Tiling", &uv.x, 0.1f))
			{
				ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeValue<Crimson::Vector2f>>(buffer.UVTiling, uv));
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
}

void MeshComponent::CreateTextureCombo(Texture*& aTexture, eTextureSlot aSlot)
{
	ImGui::PushID(aSlot);
	if (ImGui::BeginCombo("", Crimson::ToString(aTexture->GetName()).c_str(), ImGuiComboFlags_HeightLarge))
	{
		for (auto& path : AssetManager::GetAvailableTextures())
		{
			Texture* current = AssetManager::GetAsset<Texture*>(path);
			const bool isSelected = aTexture == current;
			if (ImGui::Selectable(Crimson::ToString(current->GetName()).c_str(), isSelected))
			{
				aTexture = current;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			constexpr float imageSize = 30.f;
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - imageSize);
			ImGui::Image(current->GetSRV().Get(), ImVec2(imageSize, imageSize));
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}

void MeshComponent::CreateTextureImage(Texture*& aTexture)
{
	constexpr float imageSize = 75.f;
	if (aTexture)
	{
		ImGui::Image(aTexture->GetSRV().Get(), ImVec2(imageSize, imageSize));
	}
	else
	{
		ImGui::Image(GraphicsEngine::Get().GetDefaultMaterial().GetFXTexture()->GetSRV().Get(), ImVec2(imageSize, imageSize));
	}
	AcceptDropPayload(aTexture);
}

void MeshComponent::AcceptDropPayload(Texture*& aTexture)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Texture*));
			aTexture = *static_cast<Texture**>(payload->Data);
		}
		ImGui::EndDragDropTarget();
	}
}
#endif // !_RETAIL