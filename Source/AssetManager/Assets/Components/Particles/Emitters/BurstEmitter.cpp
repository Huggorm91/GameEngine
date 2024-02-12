#include "AssetManager.pch.h"
#include "BurstEmitter.h"

BurstEmitter::BurstEmitter(): ParticleEmitter(EmitterType::Burst)
{
}

void BurstEmitter::Update(float aDeltaTime)
{
	myTimer += aDeltaTime;
	if (myTimer <= myData.LifeTime)
	{
		const float lerpValue = myTimer / myData.LifeTime;
		const float gravityPull = globalParticleGravity * aDeltaTime;

		const Crimson::Vector4f color = Crimson::Vector4f::Lerp(myData.StartColor, myData.EndColor, lerpValue);
		const Crimson::Vector3f size = Crimson::Vector3f::Lerp(myData.StartSize, myData.EndSize, lerpValue);
		const Crimson::Vector3f velocity = Crimson::Vector3f::Lerp(myData.StartVelocity, myData.EndVelocity, lerpValue);

		for (auto& particle : myParticles)
		{
			particle.myLifeTime += aDeltaTime;
			particle.myColor = color;
			particle.myScale = size;

			particle.myPosition.y -= gravityPull;
		}
	}

	if (myTimer >= myBurstInterval)
	{
		myTimer = 0.f;
		for (auto& particle : myParticles)
		{
			InitParticle(particle);
		}
	}
}
