#pragma once
#include "../Component.h"
#include <Matrix4x4.hpp>
#include "GraphicsEngine/Rendering/Texture.h"

class PointlightComponent : public Component
{
public:
	PointlightComponent();
	PointlightComponent(float aRadius, float anIntensity = 1.f, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, bool aCastShadows = true);
	PointlightComponent(const Json::Value& aJson);
	PointlightComponent(const PointlightComponent& aLight);
	~PointlightComponent() = default;
	PointlightComponent& operator=(const PointlightComponent& aLight);

	void Init(float aRadius, float anIntensity = 1.f, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, bool aCastShadows = true);
	void Update() override;

	void SetRadius(float aRadius);
	void SetIntensity(float anIntensity);
	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetColor(const CommonUtilities::Vector3f& aColor);

	float GetRadius() const;
	float GetIntensity() const;
	const CommonUtilities::Vector3f& GetPosition() const;
	const CommonUtilities::Vector3f& GetColor() const;
	CommonUtilities::Matrix4x4f GetTransform() const;

	void SetCastShadows(bool aState);
	void ToogleCastShadows();
	bool IsCastingShadows() const;
	std::shared_ptr<Texture>& GetShadowMap();

	Json::Value ToJson() const override;
	const PointlightComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myColor;
	std::shared_ptr<Texture> myShadowMap;
	float myRadius;
	float myIntensity;
	bool myCastShadows;

	void CreateShadowMap();
};