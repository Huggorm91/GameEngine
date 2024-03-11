#pragma once
#include "../Component.h"
#include "../ComponentParts/MeshElement.h"
#include "Math/Transform.h"
#include "GraphicsEngine/Rendering/Color.h"
#include "GraphicsEngine/Rendering/Texture.h"

BEGIN_COMPONENT(MeshComponent)
public:
	MeshComponent();
	MeshComponent(ComponentType aType);
	MeshComponent(const Json::Value& aJson);
	MeshComponent(const TGA::FBX::Mesh& aMesh, const std::vector<MeshElement>& anElementList, ComponentType aType = ComponentType::Mesh);
	MeshComponent(const MeshComponent& aMeshComponent);
	MeshComponent(MeshComponent&& aMeshComponent) noexcept;
	virtual ~MeshComponent() = default;
	MeshComponent& operator=(const MeshComponent& aComponent);
	MeshComponent& operator=(MeshComponent&& aComponent) noexcept;

	void Update() override;
	void Render() override;

	void Init(GameObject* aParent) override;
	void Init(const std::vector<MeshElement>& anElementList, const std::string& aName);

	void SetOffsetPosition(const Crimson::Vector3f& aPosition);
	void SetOffsetRotation(const Crimson::Vector3f& aRotation);
	void SetOffsetScale(const Crimson::Vector3f& aScale);

	const Crimson::Matrix4x4f& GetTransform() const;
	const Crimson::Vector4f& GetWorldPosition() const;

	const BoxSphereBounds& GetBounds() const;

	const std::vector<MeshElement>& GetElements() const;
	std::vector<MeshElement>& GetElements();

	void SetAlbedoTexture(Texture* aTexture);
	void SetNormalTexture(Texture* aTexture);
	void SetMaterialTexture(Texture* aTexture);
	void SetFXTexture(Texture* aTexture);

	void SetColor(const Crimson::Vector4f& aColor);
	const Crimson::Vector4f& GetColor() const;

	void SetRenderShadow(bool aState);
	void ToogleRenderShadow();
	bool IsRenderingShadow() const;

	void SetIsDeferred(bool aState);
	bool IsDeferred() const;

	const std::string& GetName() const;

	void TransformHasChanged() const override;
	void CreateImGuiComponents(const std::string& aWindowName) override;

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	Json::Value ToJson() const override;

protected:
	bool myIsDeferred;
	bool myRenderShadow;
	Crimson::Vector4f myColor;
	BoxSphereBounds myBoxSphereBounds;
	std::string myName;
	std::vector<MeshElement> myElements;
	Transform myTransform;

#ifndef _RETAIL
	void CreateMaterialImGui(Material& aMaterial);
	void CreateTextureCombo(Texture*& aTexture, eTextureSlot aSlot);
	void CreateTextureImage(Texture*& aTexture);
	void AcceptDropPayload(Texture*& aTexture);
#endif // !_RETAIL
};