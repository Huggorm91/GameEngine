#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"
#ifndef NETWORK_SERVER
#include "Emitters/BurstEmitter.h"
#include "Emitters/StreamEmitter.h"
#endif // !NETWORK_SERVER

ParticleEmitterComponent::ParticleEmitterComponent() : Component(ComponentType::ParticleEmitter), myEmitter()
{}

ParticleEmitterComponent::ParticleEmitterComponent(const Json::Value& aJson) : Component(aJson), myEmitter()
{
	// Lazy solution due to time constraints. Should be reworked
#ifndef NETWORK_SERVER
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
#endif // !NETWORK_SERVER
}

void ParticleEmitterComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	if (myEmitter)
	{
#ifndef NETWORK_SERVER
		myEmitter->SetParentTransform(*GetParentTransform());
#endif // !NETWORK_SERVER
	}
}

void ParticleEmitterComponent::Render()
{
	if (!myIsActive)
	{
		return;
	}

#ifndef NETWORK_SERVER
	myEmitter->Update(Crimson::Time::GetDeltaTime());
#endif // !NETWORK_SERVER
}

void ParticleEmitterComponent::SetEmitter(std::shared_ptr<ParticleEmitter> anEmitter)
{
#ifndef NETWORK_SERVER
	myEmitter = anEmitter;
	myEmitter->SetParentTransform(*GetParentTransform());
#endif // !NETWORK_SERVER
}

void ParticleEmitterComponent::TransformHasChanged() const
{
#ifndef NETWORK_SERVER
	myEmitter->SetParentTransform(*const_cast<Transform*>(GetParentTransform()));
#endif // !NETWORK_SERVER
}

#ifdef EDITOR
void ParticleEmitterComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
	myEmitter->CreateImGuiElements();
}
#endif // EDITOR

Json::Value ParticleEmitterComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
#ifndef NETWORK_SERVER
	result["Emitter"] = myEmitter->ToJson();
#endif // !NETWORK_SERVER
	return result;
}