#pragma once
#include "../Component.h"
#include <Vector3.hpp>
#include <Matrix4x4.hpp>

class PointlightComponent : public Component
{
public:
	PointlightComponent();
	PointlightComponent(float aRadius, float anIntensity, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f });
	~PointlightComponent() = default;

	void Init(float aRadius, float anIntensity, const CommonUtilities::Vector3f& aPosition = CommonUtilities::Vector3f::Null, const CommonUtilities::Vector3f& aColor = { 1.f, 1.f, 1.f });
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

	const PointlightComponent* GetTypePointer() const override;

private:
	float myRadius;
	float myIntensity;
	CommonUtilities::Vector3f myPosition;
	CommonUtilities::Vector3f myColor;
};