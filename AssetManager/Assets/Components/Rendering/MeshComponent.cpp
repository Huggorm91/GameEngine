#include "AssetManager.pch.h"
#include "MeshComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"

MeshComponent::MeshComponent() : Component(), myTransform(), myColor{ 0.f, 0.f, 0.f, 1.f }, myElements(), myBoxSphereBounds(), myRenderShadow(true)
{
}

MeshComponent::MeshComponent(const TGA::FBX::Mesh& aMesh) : myElements(), myName(aMesh.Name), myColor{ 0.f, 0.f, 0.f, 1.f }, myBoxSphereBounds(aMesh.BoxSphereBounds), myTransform(), myRenderShadow(true)
{
	if (aMesh.BoxBounds.IsValid && aMesh.BoxSphereBounds.Radius == 0.f)
	{
		myBoxSphereBounds = aMesh.BoxBounds;
	}
}

MeshComponent::MeshComponent(const MeshComponent& aMeshComponent) : Component(aMeshComponent), myTransform(aMeshComponent.myTransform), myColor(aMeshComponent.myColor), myElements(aMeshComponent.myElements), myBoxSphereBounds(aMeshComponent.myBoxSphereBounds), myName(aMeshComponent.myName), myRenderShadow(aMeshComponent.myRenderShadow)
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
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMesh>(*this));
}

void MeshComponent::Init(const std::vector<MeshElement>& anElementList)
{
	myElements = anElementList;
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
	return myParent->GetTransformMatrix() * myTransform.GetTransformMatrix();
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

const std::string& MeshComponent::GetName() const
{
	return myName;
}

const MeshComponent* MeshComponent::GetTypePointer() const
{
	return this;
}
