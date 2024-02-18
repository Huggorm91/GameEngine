#include "AssetManager.pch.h"
#include "StreamEmitter.h"

StreamEmitter::StreamEmitter() : ParticleEmitter(EmitterType::Stream), myTimer(0.f)
{}

StreamEmitter::StreamEmitter(const Json::Value & aJson) : ParticleEmitter(aJson), myTimer(0.f)
{}

void StreamEmitter::Update(float aDeltaTime)
{
	ParticleEmitter::Update();

	myTimer += aDeltaTime;
	const float gravityPull = GetGravity(aDeltaTime);

	auto iter = myActiveParticles.begin();
	while (iter != myActiveParticles.end())
	{
		(*iter)->myLifeTime += aDeltaTime;

		if ((*iter)->myLifeTime >= myData.LifeTime)
		{
			iter = DeactivateParticle(*iter);
		}
		else
		{
			auto& particle = *iter;

			const float lerpValue = particle->myLifeTime / myData.LifeTime;
			const float speed = Crimson::Lerp(myData.StartSpeed, myData.EndSpeed, lerpValue) * aDeltaTime;
			particle->myColor = Crimson::Vector4f::Lerp(myData.StartColor, myData.EndColor, lerpValue);
			particle->myScale = Crimson::Vector3f::Lerp(myData.StartSize, myData.EndSize, lerpValue);

			const Crimson::Vector3f& change = particle->myDirection * speed;
			particle->myPosition += change;
			particle->myPosition.y -= gravityPull;

			iter++;
		}		
	}

	if (myTimer >= 1.f / myData.SpawnRate && !myInactiveParticles.empty())
	{
		myTimer = 0.f;
		ActivateParticle(myInactiveParticles.back());
	}
}

void StreamEmitter::CreateImGuiElements()
{
	ImGui::Text("Stream Emitter");
	ParticleEmitter::CreateImGuiElements();
}
