#pragma once
#include "LightCommand.h"
#include "AssetManager/Assets/Components/Light/PointlightComponent.h"

class LitCmd_AddPointlight : public LightCommand
{
public:
	LitCmd_AddPointlight(PointlightComponent& aLight);
	~LitCmd_AddPointlight() override = default;

	void Execute(const int anIndex) override;

	Texture* GetShadowMap() override;
	void SetShadowMap(const int anIndex) override;
	bool CastsShadow() const override;

private:
	bool myCastsShadow;
	float myRadius;
	float myIntensity;
	std::shared_ptr<Texture> myShadowMap;
	Crimson::Vector3f myPosition;
	Crimson::Vector3f myColor;
};

typedef LitCmd_AddPointlight GfxCmd_AddPointlight;