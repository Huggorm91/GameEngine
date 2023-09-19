#pragma once
#include "../Component.h"
#include "GraphicsEngine/Drawer/LineDrawer.h"

class DebugDrawComponent : public Component
{
public:

	DebugDrawComponent();
	DebugDrawComponent(const Json::Value& aJson, GameObject* aParent);
	~DebugDrawComponent() = default;

	void Update() override;

	void SetActive(bool aIsActive) override;
	void ToogleActive() override;

	void SetLine(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, const CommonUtilities::Vector4f& aColor = ColorManager::GetColor("White"), bool aIsUI = false);
	void SetAxisLines(const CommonUtilities::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const CommonUtilities::Vector3f& aXDirection = { 1.f, 0.f, 0.f }, const CommonUtilities::Vector3f& aYDirection = { 0.f, 1.f, 0.f }, const CommonUtilities::Vector3f& aZDirection = { 0.f, 0.f, 1.f }, const CommonUtilities::Vector4f& aXColor = ColorManager::GetColor("Red"), const CommonUtilities::Vector4f& aYColor = ColorManager::GetColor("Green"), const CommonUtilities::Vector4f& aZColor = ColorManager::GetColor("Blue"), bool aIsUI = false);
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

	//Json::Value ToJson() const override;
	const DebugDrawComponent* GetTypePointer() const override;

private:
	LineDrawer::LineHandle myHandle;
	/*enum class DrawType
	{
		Line,
		Axis,
		Arrow,
		Square,
		Circle,
		Triangle,
		Cube,
		Sphere,
		Cylinder,
		Capsule,
		Cone
	};
	DrawType myType;*/
	// int myValueCount;
	// CommonUtilities::Blackboard<int> myValues;

	// void AddToValues(Json::Value aJson); // Adds a value to blackboard, ["Key"] ["Value"]
	// void CreateHandleFromValues(); // Calls the appropiate Set-function depending on myType
};