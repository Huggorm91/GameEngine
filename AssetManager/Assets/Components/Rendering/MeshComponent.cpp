#include "AssetManager.pch.h"
#include "MeshComponent.h"
#include "Assets/GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_SetObjectBuffer.h"

MeshComponent::MeshComponent() : Component(), myTransform(), myColor(), myElements(), myBoxSphereBounds()
{
}

MeshComponent::MeshComponent(const TGA::FBX::Mesh& aMesh) : myElements(), myName(aMesh.Name), myColor(), myBoxSphereBounds(aMesh.BoxSphereBounds), myTransform()
{
	if (aMesh.BoxBounds.IsValid && aMesh.BoxSphereBounds.Radius == 0.f)
	{
		myBoxSphereBounds = aMesh.BoxBounds;
	}
}

MeshComponent::MeshComponent(const MeshComponent& aMeshComponent) : Component(aMeshComponent), myTransform(aMeshComponent.myTransform), myColor(aMeshComponent.myColor), myElements(aMeshComponent.myElements), myBoxSphereBounds(aMeshComponent.myBoxSphereBounds), myName(aMeshComponent.myName)
{
}

void MeshComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetObjectBuffer>(*this));
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
	return myParent->GetTransformMatrix() * myTransform.GetTransform();
}

const std::vector<MeshElement>& MeshComponent::GetElements() const
{
	return myElements;
}

std::vector<MeshElement>& MeshComponent::GetElements()
{
	return myElements;
}

void MeshComponent::SetTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetTexture(aTexture);
	}
}

void MeshComponent::SetNormalTexture(Texture* aTexture)
{
	for (auto& element : myElements)
	{
		element.myMaterial.SetNormalTexture(aTexture);
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

const std::string& MeshComponent::GetName() const
{
	return myName;
}

const MeshComponent* MeshComponent::GetTypePointer() const
{
	return this;
}
