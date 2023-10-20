#pragma once
#include "../Component.h"
#include "Math/Matrix4x4.hpp"
#include "GraphicsEngine/Rendering/Texture.h"

class PointlightComponent : public Component
{
public:
	PointlightComponent();
	PointlightComponent(float aRadius, float anIntensity = 1.f, const Crimson::Vector3f& aColor = { 1.f, 1.f, 1.f }, const Crimson::Vector3f& aPosition = Crimson::Vector3f::Null, bool aCastShadows = true);
	PointlightComponent(const Json::Value& aJson);
	PointlightComponent(const PointlightComponent& aLight);
	PointlightComponent(PointlightComponent&& aLight) = default;
	~PointlightComponent() = default;
	PointlightComponent& operator=(const PointlightComponent& aLight);
	PointlightComponent& operator=(PointlightComponent&& aLight) = default;

	void Init(float aRadius, float anIntensity = 1.f, const Crimson::Vector3f& aColor = { 1.f, 1.f, 1.f }, const Crimson::Vector3f& aPosition = Crimson::Vector3f::Null, bool aCastShadows = true);
	void Update() override;
	void Render() override;

	void SetRadius(float aRadius);
	void SetIntensity(float anIntensity);
	void SetPosition(const Crimson::Vector3f& aPosition);
	void SetColor(const Crimson::Vector3f& aColor);

	float GetRadius() const;
	float GetIntensity() const;
	const Crimson::Vector3f& GetPosition() const;
	const Crimson::Vector3f& GetColor() const;
	Crimson::Matrix4x4f GetTransform() const;

	void SetCastShadows(bool aState);
	void ToogleCastShadows();
	bool IsCastingShadows() const;
	std::shared_ptr<Texture>& GetShadowMap();

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const PointlightComponent* GetTypePointer() const override;

private:
	std::shared_ptr<Texture> myShadowMap;
	Crimson::Vector3f myPosition;
	Crimson::Vector3f myColor;
	float myRadius;
	float myIntensity;
	bool myCastShadows;

	void CreateShadowMap();
};