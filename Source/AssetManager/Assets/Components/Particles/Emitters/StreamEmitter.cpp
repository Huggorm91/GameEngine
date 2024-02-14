#include "AssetManager.pch.h"
#include "StreamEmitter.h"

StreamEmitter::StreamEmitter() : ParticleEmitter(EmitterType::Stream)
{}

StreamEmitter::StreamEmitter(const Json::Value & aJson) : ParticleEmitter(aJson)
{}

void StreamEmitter::Update(float aDeltaTime)
{
	const float gravityPull = globalParticleGravity * aDeltaTime;

	for (auto& particle : myParticles)
	{
		particle.myLifeTime += aDeltaTime;

		if (particle.myLifeTime >= myData.LifeTime)
		{
			InitParticle(particle);
		}
		else
		{
			const float lerpValue = particle.myLifeTime / myData.LifeTime;
			const float speed = Crimson::Lerp(myData.StartSpeed, myData.EndSpeed, lerpValue) * aDeltaTime;
			particle.myColor = Crimson::Vector4f::Lerp(myData.StartColor, myData.EndColor, lerpValue);
			particle.myScale = Crimson::Vector3f::Lerp(myData.StartSize, myData.EndSize, lerpValue);

			Crimson::Vector3f change = particle.myDirection * speed;
			particle.myPosition += change;
			particle.myPosition.y -= gravityPull;
		}		
	}
}