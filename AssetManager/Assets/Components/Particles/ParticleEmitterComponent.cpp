#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent(): Component(ComponentType::ParticleEmitter), myTransform(), myEmitters()
{
}

ParticleEmitterComponent::ParticleEmitterComponent(const Json::Value& aJson)
{
	assert(!"Not Implemented");
}

void ParticleEmitterComponent::Init(GameObject* aParent)
{
	assert(!"Not Implemented");
}

void ParticleEmitterComponent::Update()
{
	assert(!"Not Implemented");
}

void ParticleEmitterComponent::TransformHasChanged() const
{
	assert(!"Not Implemented");
}

void ParticleEmitterComponent::AddEmitter()
{
	myEmitters.emplace_back(ParticleEmitter(static_cast<unsigned>(myEmitters.size())));
}

void ParticleEmitterComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	assert(!"Not Implemented");
}

Json::Value ParticleEmitterComponent::ToJson() const
{
	Json::Value result;
	result["Transform"] = myTransform.ToJson();
	result["Emitters"] = Json::arrayValue;
	int index = 0;
	for (auto& emitter : myEmitters)
	{
		result["Emitters"][index] = emitter.ToJson();
	}
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
