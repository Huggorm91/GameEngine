#pragma once
#include "../Component.h"
#include "../Transform.h"
#include "../ComponentParts/MeshElement.h"
#include "GraphicsEngine/Rendering/Color.h"
#include "GraphicsEngine/Rendering/Texture.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	MeshComponent(ComponentType aType);
	MeshComponent(const TGA::FBX::Mesh& aMesh, const std::vector<MeshElement>& anElementList, const std::string* aPath, ComponentType aType = ComponentType::Mesh);
	MeshComponent(const MeshComponent& aMeshComponent);
	MeshComponent(MeshComponent&& aMeshComponent) = default;
	virtual ~MeshComponent() = default;
	MeshComponent& operator=(const MeshComponent& aComponent) = default;
	MeshComponent& operator=(MeshComponent&& aComponent) noexcept = default;

	void Update() override;

	void Init(const Json::Value& aJson) override;
	void Init(const std::vector<MeshElement>& anElementList, const std::string& aName, const std::string* aPath);

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
	void SetFXTexture(Texture* aTexture);

	void SetColor(const CommonUtilities::Vector4f& aColor);
	const CommonUtilities::Vector4f& GetColor() const;

	void SetRenderShadow(bool aState);
	void ToogleRenderShadow();
	bool IsRenderingShadow() const;

	void SetIsDeferred(bool aState);
	bool IsDeferred() const;

	void BoundsHasChanged() const;
	const std::string& GetName() const;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	const MeshComponent* GetTypePointer() const override;

protected:
	bool myIsDeferred;
	bool myRenderShadow;
	const std::string* myPath;
#ifndef _RETAIL
	float myLerpValue;
	CommonUtilities::Vector4f myLerpColor1;
	CommonUtilities::Vector4f myLerpColor2;
	std::string myLerpName1;
	std::string myLerpName2;
#endif // !_RETAIL
	CommonUtilities::Vector4f myColor;
	std::string myName;
	BoxSphereBounds myBoxSphereBounds;
	CommonUtilities::Matrix4x4f myTransformMatrix;
	std::vector<MeshElement> myElements;
	Transform myTransform;
};