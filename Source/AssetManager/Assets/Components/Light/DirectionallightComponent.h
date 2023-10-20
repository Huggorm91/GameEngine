#pragma once
#include "../Component.h"
#include "Math/Vector3.hpp"
#include "GraphicsEngine/Rendering/Texture.h"

class DirectionallightComponent : public Component
{
public:
	DirectionallightComponent();
	DirectionallightComponent(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& aColor = { 1.f, 1.f, 1.f }, float anIntensity = 1.f, bool aCastShadows = true);
	DirectionallightComponent(const Json::Value& aJson);
	DirectionallightComponent(const DirectionallightComponent& aLight);
	DirectionallightComponent(DirectionallightComponent&& aLight) = default;
	~DirectionallightComponent() = default;
	DirectionallightComponent& operator=(const DirectionallightComponent& aLight);
	DirectionallightComponent& operator=(DirectionallightComponent&& aLight) = default;

	void Init(const Crimson::Vector3f& aDirection, const Crimson::Vector3f& aColor = { 1.f, 1.f, 1.f }, float anIntensity = 1.f, bool aCastShadows = true);
	void Update() override;
	void Render() override;

	void SetLightDirection(const Crimson::Vector3f& aDirection);
	void SeColor(const Crimson::Vector3f& aColor);
	void SetIntensity(float anIntensity);

	Crimson::Vector3f GetLightDirection() const;
	const Crimson::Vector3f& GetInvertedLightDirection() const;
	const Crimson::Vector3f& GetColor() const;
	float GetIntensity() const;

	void SetCastShadows(bool aState);
	void ToogleCastShadows();
	bool IsCastingShadows() const;
	std::shared_ptr<Texture>& GetShadowMap();

	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const DirectionallightComponent* GetTypePointer() const override;

private:
	std::shared_ptr<Texture> myShadowMap;
	Crimson::Vector3f myInvertedLightDirection;
	Crimson::Vector3f myLightDirection;
	Crimson::Vector3f myColor;
	float myIntensity;
	bool myCastShadows;

	void CreateShadowMap();
};