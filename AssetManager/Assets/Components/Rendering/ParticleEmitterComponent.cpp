#include "AssetManager.pch.h"
#include "ParticleEmitterComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent(): Component(ComponentType::ParticleEmitter), myTransform()
{
}

inline std::string ParticleEmitterComponent::ToString() const
{
	return "ParticleEmitter";
}

const ParticleEmitterComponent* ParticleEmitterComponent::GetTypePointer() const
{
	return this;
}
