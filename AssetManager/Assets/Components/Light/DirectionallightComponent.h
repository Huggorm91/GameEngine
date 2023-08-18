#pragma once
#include "../Component.h"
#include <Vector3.hpp>

class DirectionallightComponent : public Component
{
public:
	DirectionallightComponent();
	DirectionallightComponent(const CommonUtilities::Vector3f& aDirection, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f }, float anIntensity = 1.f);
	~DirectionallightComponent() = default;

	void Update() override;

	void SetLightDirection(const CommonUtilities::Vector3f& aDirection);
	void SeColor(const CommonUtilities::Vector3f& aColor);
	void SetIntensity(float anIntensity);

	CommonUtilities::Vector3f GetLightDirection() const;
	const CommonUtilities::Vector3f& GetInvertedLightDirection() const;
	const CommonUtilities::Vector3f& GetColor() const;
	float GetIntensity() const;

	const DirectionallightComponent* GetTypePointer() const override;

private:
	CommonUtilities::Vector3f myInvertedLightDirection;
	CommonUtilities::Vector3f myColor;
	float myIntensity;
};