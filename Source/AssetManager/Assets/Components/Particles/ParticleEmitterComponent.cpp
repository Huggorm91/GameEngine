#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"
#include "Time/Timer.h"
#include "Emitters/BurstEmitter.h"
#include "Emitters/StreamEmitter.h"

ParticleEmitterComponent::ParticleEmitterComponent() : Component(ComponentType::ParticleEmitter), myEmitter()
{}

ParticleEmitterComponent::ParticleEmitterComponent(const Json::Value& aJson): Component(aJson), myEmitter()
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
}

void ParticleEmitterComponent::Update()
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
}

void ParticleEmitterComponent::CreateImGuiComponents(const std::string&)
{
	assert(!"Not Implemented");
}

Json::Value ParticleEmitterComponent::ToJson() const
{
	Json::Value result;
	result["Emitter"] = myEmitter->ToJson();
	return result;
}

inline std::string ParticleEmitterComponent::ToString() const
{
	return "ParticleEmitter";
}

const ParticleEmitterComponent* ParticleEmitterComponent::GetTypePointer() const
{
	return this;
}
