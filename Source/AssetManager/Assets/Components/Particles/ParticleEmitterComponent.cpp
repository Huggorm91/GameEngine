#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"
#include "Emitters/BurstEmitter.h"
#include "Emitters/StreamEmitter.h"

ParticleEmitterComponent::ParticleEmitterComponent() : Component(ComponentType::ParticleEmitter), myEmitter()
{}

ParticleEmitterComponent::ParticleEmitterComponent(const Json::Value& aJson) : Component(aJson), myEmitter()
{
	// Lazy solution due to time constraints. Should be reworked
	ParticleEmitter::EmitterType type = static_cast<ParticleEmitter::EmitterType>(aJson["Emitter"]["Type"].asInt());
	switch (type)
	{
	case ParticleEmitter::EmitterType::Burst:
	{
		myEmitter = std::make_shared<BurstEmitter>(aJson["Emitter"]);
		break;
	}
	case ParticleEmitter::EmitterType::Stream:
	{
		myEmitter = std::make_shared<StreamEmitter>(aJson["Emitter"]);
		break;
	}
	default:
		break;
	}

	myEmitter->InitAfterJsonLoad();
}

void ParticleEmitterComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	if (myEmitter)
	{
		myEmitter->SetParentTransform(*GetParentTransform());
	}
}

void ParticleEmitterComponent::Update()
{
	Render();
}

void ParticleEmitterComponent::Render()
{
	if (!myIsActive)
	{
		return;
	}

	myEmitter->Update(Crimson::Timer::GetDeltaTime());
}

void ParticleEmitterComponent::SetEmitter(std::shared_ptr<ParticleEmitter> anEmitter)
{
	myEmitter = anEmitter;
	myEmitter->SetParentTransform(*GetParentTransform());
}

void ParticleEmitterComponent::TransformHasChanged() const
{
	myEmitter->SetParentTransform(*const_cast<Transform*>(GetParentTransform()));
}

void ParticleEmitterComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	myEmitter->CreateImGuiElements();
}

Json::Value ParticleEmitterComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["Emitter"] = myEmitter->ToJson();
	return result;
}