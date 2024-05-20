#pragma once
#include "../Component.h"
#include "GraphicsEngine/Drawer/LineHandle.h"
#include "GraphicsEngine/Rendering/Color.h"

BEGIN_COMPONENT(DebugDrawComponent)
public:

	DebugDrawComponent();
	DebugDrawComponent(const Json::Value& aJson, GameObject* aParent);
	DebugDrawComponent(const DebugDrawComponent& aLight) = default;
	DebugDrawComponent(DebugDrawComponent&& aLight) = default;
	~DebugDrawComponent() = default;
	DebugDrawComponent& operator=(const DebugDrawComponent& aLight) = default;
	DebugDrawComponent& operator=(DebugDrawComponent&& aLight) = default;

	void TransformHasChanged() const override;

	void SetActive(bool aIsActive) override;
	void ToogleActive() override;

	void SetLine(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, const Crimson::Vector4f& aColor = ColorManager::GetColor("White"), bool aIsUI = false);
	void SetAxisLines(const Crimson::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const Crimson::Vector3f& aXDirection = { 1.f, 0.f, 0.f }, const Crimson::Vector3f& aYDirection = { 0.f, 1.f, 0.f }, const Crimson::Vector3f& aZDirection = { 0.f, 0.f, 1.f }, const Crimson::Vector4f& aXColor = ColorManager::GetColor("Red"), const Crimson::Vector4f& aYColor = ColorManager::GetColor("Green"), const Crimson::Vector4f& aZColor = ColorManager::GetColor("Blue"), bool aIsUI = false);
	//void SetArrow(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSquare(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSquare(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCircle(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetTriangle(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCube(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCube(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetSphere(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCylinder(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCapsule(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);
	//void SetCone(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), bool aIsUI = false);

	//Json::Value ToJson() const override;

private:
	LineHandle myHandle;
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
	// Crimson::Blackboard<int> myValues;

	// void AddToValues(Json::Value aJson); // Adds a value to blackboard, ["Key"] ["Value"]
	// void CreateHandleFromValues(); // Calls the appropiate Set-function depending on myType
};