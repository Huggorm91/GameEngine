#pragma once
#include "../Component.h"
#include "../Transform.h"
#include "../ComponentParts/MeshElement.h"
#include "SkeletonComponent.h"
#include "GraphicsEngine/Rendering/Color.h"
#include "GraphicsEngine/Rendering/Texture.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	MeshComponent(const TGA::FBX::Mesh& aMesh);
	MeshComponent(const MeshComponent& aMeshComponent);
	virtual ~MeshComponent() = default;

	void Update() override;
	void Init(const std::vector<MeshElement>& anElementList);

	void SetOffsetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetOffsetRotation(const CommonUtilities::Vector3f& aRotation);
	void SetOffsetScale(const CommonUtilities::Vector3f& aScale);

	CommonUtilities::Matrix4x4f GetTransform() const;
	CommonUtilities::Vector4f GetWorldPosition() const;

	const std::vector<MeshElement>& GetElements() const;
	std::vector<MeshElement>& GetElements();

	void SetAlbedoTexture(Texture* aTexture);
	void SetNormalTexture(Texture* aTexture);
	void SetMaterialTexture(Texture* aTexture);

	void SetColor(const CommonUtilities::Vector4f& aColor);
	const CommonUtilities::Vector4f& GetColor() const;

	void SetRenderShadow(bool aState);
	void ToogleRenderShadow();

	const std::string& GetName() const;

	const MeshComponent* GetTypePointer() const override;

protected:
	bool myRenderShadow;
	CommonUtilities::Vector4f myColor;
	std::string myName;
	BoxSphereBounds myBoxSphereBounds;
	std::vector<MeshElement> myElements;
	Transform myTransform;

	friend class ModelManager;
};