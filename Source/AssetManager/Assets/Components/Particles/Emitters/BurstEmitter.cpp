#include "AssetManager.pch.h"
#include "BurstEmitter.h"
#include "Math/Matrix3x3.hpp"

BurstEmitter::BurstEmitter() :
	ParticleEmitter(EmitterType::Burst),
	myBurstInterval(0.f),
	myTimer(0.f),
	myIsLooping(false),
	myIsActive(false)
{}

BurstEmitter::BurstEmitter(const Json::Value& aJson) :
	ParticleEmitter(aJson),
	myIsLooping(aJson["IsLooping"].asBool()),
	myIsActive(aJson["IsActive"].asBool())
{}

void BurstEmitter::Update(float aDeltaTime)
{
	if (!myIsActive)
	{
		return;
	}

	myTimer += aDeltaTime;
	if (myTimer <= myData.LifeTime)
	{
		UpdateParticles(aDeltaTime);
	}

	if (myTimer >= myBurstInterval)
	{
		myTimer = 0.f;
		if (!myIsLooping)
		{
			myIsActive = false;
			return;
		}
		CreateBurst();
	}
}

void BurstEmitter::CreateImGuiElements()
{
	assert(!"Not Implemented");
}

Json::Value BurstEmitter::ToJson() const
{
	auto result = ParticleEmitter::ToJson();
	result["Interval"] = myBurstInterval;
	result["IsLooping"] = myIsLooping;
	result["IsActive"] = myIsActive;
	return result;
}

void BurstEmitter::UpdateParticles(float aDeltaTime)
{
	const float lerpValue = myTimer / myData.LifeTime;
	const float gravityPull = globalParticleGravity * aDeltaTime;

	const float speed = Crimson::Lerp(myData.StartSpeed, myData.EndSpeed, lerpValue) * aDeltaTime;
	const Crimson::Vector4f color = Crimson::Vector4f::Lerp(myData.StartColor, myData.EndColor, lerpValue);
	const Crimson::Vector3f size = Crimson::Vector3f::Lerp(myData.StartSize, myData.EndSize, lerpValue);

	for (auto& particle : myParticles)
	{
		particle.myLifeTime += aDeltaTime;
		particle.myColor = color;
		particle.myScale = size;

		Crimson::Vector3f change = particle.myDirection * speed;
		particle.myPosition += change;
		particle.myPosition.y -= gravityPull;
	}
}

void BurstEmitter::CreateBurst()
{
	const float angleIncrease = 360.f / myParticles.size();
	float angle = 0.f;
	for (auto& particle : myParticles)
	{
		InitParticle(particle);
		particle.myDirection = Crimson::Matrix3x3f::CreateRotationAroundX(angle) * Crimson::Vector3f::Forward;
		angle += angleIncrease;
	}
}
