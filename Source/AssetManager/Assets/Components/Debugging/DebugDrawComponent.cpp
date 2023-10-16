#include "AssetManager.pch.h"
#include "DebugDrawComponent.h"
#include "../../GameObject.h"
#include "GraphicsEngine/GraphicsEngine.h"

DebugDrawComponent::DebugDrawComponent() : Component(ComponentType::DebugDraw)
#ifndef _RETAIL
	, myHandle(GraphicsEngine::Get().GetLineDrawer().GetNewHandle())
#else
	myHandle()
#endif // !_RETAIL	
{
}

void DebugDrawComponent::Update()
{
#ifndef _RETAIL
	myHandle.UpdateTransform(myParent->GetTransformMatrix());
#endif // !_RETAIL
}

void DebugDrawComponent::SetActive(bool aIsActive)
{
	Component::SetActive(aIsActive);
#ifndef _RETAIL
	myHandle.SetActive(aIsActive);
#endif // !_RETAIL	
}

void DebugDrawComponent::ToogleActive()
{
	Component::ToogleActive();
#ifndef _RETAIL
	myHandle.SetActive(myIsActive);	
#endif // !_RETAIL	
}

void DebugDrawComponent::SetLine(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, const Crimson::Vector4f& aColor, bool aIsUI)
{
#ifndef _RETAIL
	assert(myParent != nullptr && "DebugDrawerComponent not Initialized!");
	GraphicsEngine::Get().GetLineDrawer().AddLine(aFrom, aTo, aColor, myParent->GetTransformMatrix(), aIsUI, &myHandle);
	myHandle.SetActive(true);
#else
	UNREFERENCED_PARAMETER(aFrom);
	UNREFERENCED_PARAMETER(aTo);
	UNREFERENCED_PARAMETER(aColor);
	UNREFERENCED_PARAMETER(aIsUI);
#endif // !_RETAIL	
}

void DebugDrawComponent::SetAxisLines(const Crimson::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const Crimson::Vector3f& aXDirection, const Crimson::Vector3f& aYDirection, const Crimson::Vector3f& aZDirection, const Crimson::Vector4f& aXColor, const Crimson::Vector4f& aYColor, const Crimson::Vector4f& aZColor, bool aIsUI)
{
#ifndef _RETAIL
	assert(myParent != nullptr && "DebugDrawerComponent not Initialized!");
	GraphicsEngine::Get().GetLineDrawer().AddAxisLines(aCenter, aLineLength, aIsAxisPointingBothWays, aXDirection, aYDirection, aZDirection, aXColor, aYColor, aZColor, myParent->GetTransformMatrix(), aIsUI, &myHandle);
	myHandle.SetActive(true);
#else
	UNREFERENCED_PARAMETER(aCenter);
	UNREFERENCED_PARAMETER(aLineLength);
	UNREFERENCED_PARAMETER(aIsAxisPointingBothWays);
	UNREFERENCED_PARAMETER(aXDirection);
	UNREFERENCED_PARAMETER(aYDirection);
	UNREFERENCED_PARAMETER(aZDirection);
	UNREFERENCED_PARAMETER(aXColor);
	UNREFERENCED_PARAMETER(aYColor);
	UNREFERENCED_PARAMETER(aZColor);
	UNREFERENCED_PARAMETER(aIsUI);
#endif // !_RETAIL	
}

inline std::string DebugDrawComponent::ToString() const
{
	return "DebugDraw";
}

const DebugDrawComponent* DebugDrawComponent::GetTypePointer() const
{
	return this;
}
