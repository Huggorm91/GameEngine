#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"
#include "Time/Timer.h"

ParticleEmitterComponent::ParticleEmitterComponent() : Component(ComponentType::ParticleEmitter), myEmitter()
{}

ParticleEmitterComponent::ParticleEmitterComponent(const Json::Value& aJson): Component(aJson), myEmitter()
{
}

void ParticleEmitterComponent::Update()
{
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
