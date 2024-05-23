#pragma once
#include "../Component.h"
#include "CrimsonUtilities/Math/Matrix4x4.hpp"
#include "GraphicsEngine/Rendering/Texture.h"

BEGIN_COMPONENT(SpotlightComponent)
public:
	SpotlightComponent();
	SpotlightComponent(float aRange, float anIntensity, float anInnerAngle, float anOuterAngle, const Crimson::Vector3f& aDirection, const Crimson::Vector3f& aPosition = Crimson::Vector3f::Null, const Crimson::Vector3f& aColor = { 1.f, 1.f, 1.f }, bool aCastShadows = true);
	SpotlightComponent(const Json::Value& aJson);
	SpotlightComponent(const SpotlightComponent& aLight);
	SpotlightComponent(SpotlightComponent&& aLight) = default;
	~SpotlightComponent() = default;
	SpotlightComponent& operator=(const SpotlightComponent& aLight);
	SpotlightComponent& operator=(SpotlightComponent&& aLight) = default;

	void Update() override;
	void Render() override;

	void SetRange(float aRange);
	void SetIntensity(float anIntensity);
	void SetInnerAngle(float anAngle);
	void SetOuterAngle(float anAngle);
	void SetPosition(const Crimson::Vector3f& aPosition);
	void SetLightDirection(const Crimson::Vector3f& aDirection);
	void SetColor(const Crimson::Vector3f& aColor);

	float GetRange() const;
	float GetIntensity() const;
	float GetInnerAngle() const;
	float GetOuterAngle() const;
	const Crimson::Vector3f& GetPosition() const;
	const Crimson::Vector3f& GetLightDirection() const;
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

private:
#ifndef _RETAIL
	Crimson::Vector3f myEditDirection;
#endif // !_RETAIL
	std::shared_ptr<Texture> myShadowMap;
	Crimson::Vector3f myPosition;
	Crimson::Vector3f myLightDirection;
	Crimson::Vector3f myColor;
	float myRange;
	float myIntensity;
	float myInnerAngle;
	float myOuterAngle;
	bool myCastShadows;

	void CreateShadowMap();
};