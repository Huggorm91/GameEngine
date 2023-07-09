#pragma once
#include "../Component.h"
#include "GraphicsEngine/Drawer/LineDrawer.h"

class DebugDrawComponent : public Component
{
public:

	DebugDrawComponent();
	~DebugDrawComponent() = default;

	void Update() override;

	void SetActive(bool aIsActive) override;
	void ToogleActive() override;

	void SetLine(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	void SetAxisLines(const CommonUtilities::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const CommonUtilities::Vector3f& aXDirection = { 1.f, 0.f, 0.f }, const CommonUtilities::Vector3f& aYDirection = { 0.f, 1.f, 0.f }, const CommonUtilities::Vector3f& aZDirection = { 0.f, 0.f, 1.f }, const CommonUtilities::Vector4f& aXColor = GetColor(eColor::Red), const CommonUtilities::Vector4f& aYColor = GetColor(eColor::Green), const CommonUtilities::Vector4f& aZColor = GetColor(eColor::Blue), bool aIsUI = false);
	//void SetArrow(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSquare(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSquare(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCircle(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetTriangle(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCube(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCube(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSphere(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCylinder(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCapsule(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCone(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);

	const DebugDrawComponent* GetTypePointer() const override;

private:
	LineDrawer::LineHandle myHandle;
};