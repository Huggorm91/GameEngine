#include "AssetManager.pch.h"
#include "MeshComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"
#include "GraphicsEngine/Commands/GfxCmd_UpdateWorldBounds.h"

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
}

void MeshComponent::Update()
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

void MeshComponent::Init(const Json::Value& aJson)
{
	Component::Init(aJson);
	myRenderShadow = aJson["RenderShadow"].asBool();
	myColor = CommonUtilities::Vector4f(aJson["Color"]);
	myTransform = aJson["Transform"];
	myIsDeferred = aJson["IsDeferred"].asBool();
	for (auto& element : aJson["Elements"])
	{
		myElements[element["Index"].asInt()].myMaterial = element["Material"];
	}

#ifndef _RETAIL
	myLerpValue = aJson["LerpValue"].asFloat();
	myLerpColor1 = CommonUtilities::Vector4f(aJson["LerpColor1"]);
	myLerpColor2 = CommonUtilities::Vector4f(aJson["LerpColor2"]);
#endif // !_RETAIL
}

void MeshComponent::Init(const std::vector<MeshElement>& anElementList, const std::string& aName, const std::string* aPath)
{
	myElements = anElementList;
	myName = aName;
	myPath = aPath;

	CommonUtilities::Vector3f min;
	CommonUtilities::Vector3f max;

	for (auto& element : myElements)
	{
		const CommonUtilities::Vector3f& elementMin = element.myData->myBoxSphereBounds.GetMin();
		const CommonUtilities::Vector3f& elementMax = element.myData->myBoxSphereBounds.GetMax();
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

	CommonUtilities::Vector3f center = (min + max) * 0.5f;
	CommonUtilities::Vector3f size = max - min;
	myBoxSphereBounds.Init(center, size);
}

void MeshComponent::SetOffsetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void MeshComponent::SetOffsetRotation(const CommonUtilities::Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void MeshComponent::SetOffsetScale(const CommonUtilities::Vector3f& aScale)
{
	myTransform.SetScale(aScale);
}

CommonUtilities::Matrix4x4f MeshComponent::GetTransform() const
{
	assert(myParent != nullptr && "MeshComponent is not Initialized!");
	if (myTransform.HasChanged())
	{
		BoundsHasChanged();
	}
	return myTransformMatrix;
}

CommonUtilities::Vector4f MeshComponent::GetWorldPosition() const
{
	return myParent->GetTransformMatrix() * CommonUtilities::Vector4f{ myTransform.GetWorldPosition(), 1.f };
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

void MeshComponent::SetColor(const CommonUtilities::Vector4f& aColor)
{
	myColor = aColor;
}

const CommonUtilities::Vector4f& MeshComponent::GetColor() const
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

void MeshComponent::BoundsHasChanged() const
{
	const_cast<CommonUtilities::Matrix4x4f&>(myTransformMatrix) = CommonUtilities::Matrix4x4f::CreateScaleMatrix(myParent->GetTransform().GetScale() * myTransform.GetScale()) *
		CommonUtilities::Matrix4x4f::CreateRotationMatrix(CommonUtilities::DegreeToRadian(myParent->GetTransform().GetRotation() + myTransform.GetRotation())) *
		CommonUtilities::Matrix4x4f::CreateTranslationMatrix(myParent->GetTransform().GetPosition() + myTransform.GetPosition());
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_UpdateWorldBounds>(myTransformMatrix * CommonUtilities::Vector4f(myBoxSphereBounds.GetMin(), 1.f), myTransformMatrix * CommonUtilities::Vector4f(myBoxSphereBounds.GetMax(), 1.f)));
}

const std::string& MeshComponent::GetName() const
{
	return myName;
}

void MeshComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	ImGui::Checkbox("Render Shadow", &myRenderShadow);
	if (ImGui::ColorEdit4("Color1", (float*)&myLerpColor1))
	{
		myColor = CommonUtilities::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	if (ImGui::ColorEdit4("Color2", (float*)&myLerpColor2))
	{
		myColor = CommonUtilities::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	if (ImGui::SliderFloat("Color", &myLerpValue, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
	{
		myColor = CommonUtilities::Vector4f::Lerp(myLerpColor1, myLerpColor2, myLerpValue);
	}
	ImGui::SameLine();
	ImGui::ColorEdit4("  ", (float*)&myColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);
	ImGui::Checkbox("Is Deferred Rendered", &myIsDeferred);
	ImGui::ColorEdit4("Color", (float*)&myColor);
	// Add editor for Elements
	myTransform.CreateImGuiComponents(aWindowName);
}

Json::Value MeshComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["RenderShadow"] = myRenderShadow;
	result["IsDeferred"]= myIsDeferred;
	result["Color"] = static_cast<Json::Value>(myColor);
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
	result["LerpColor1"] = static_cast<Json::Value>(myLerpColor1);
	result["LerpColor2"] = static_cast<Json::Value>(myLerpColor2);
#endif // !_RETAIL

	return result;
}

const MeshComponent* MeshComponent::GetTypePointer() const
{
	return this;
}