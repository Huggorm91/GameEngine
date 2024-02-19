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
	myBurstInterval(aJson["Interval"].asFloat()),
	myTimer(myBurstInterval),
	myIsLooping(aJson["IsLooping"].asBool()),
	myIsActive(aJson["IsActive"].asBool())
{}

void BurstEmitter::Init(float aBurstInterval, const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader)
{
	myBurstInterval = aBurstInterval;
	myTimer = aBurstInterval;
	ParticleEmitter::Init(someData, aTexture, aVertexShader, aGeometryShader, aPixelShader);
}

void BurstEmitter::Update(float aDeltaTime)
{
	ParticleEmitter::Update();
	UpdateParticles(aDeltaTime);

	if (!myIsActive)
	{
		return;
	}

	myTimer += aDeltaTime;

	if (myTimer >= myBurstInterval)
	{
		myTimer = 0.f;
		CreateBurst();
		if (!myIsLooping)
		{
			myIsActive = false;
			return;
		}
	}
}

void BurstEmitter::SetLooping(bool aState)
{
	myIsLooping = aState;
}

void BurstEmitter::TriggerBurst()
{
	myIsActive = true;
	CreateBurst();
}

void BurstEmitter::CreateImGuiElements()
{
	ImGui::Text("Burst Emitter");
	ParticleEmitter::CreateImGuiElements();

	ImGui::DragFloat("Burst Interval", &myBurstInterval);
	ImGui::Checkbox("Is Looping", &myIsLooping);

	if (ImGui::Button("Start"))
	{
		myIsActive = true;
	}
}

Json::Value BurstEmitter::ToJson() const
{
	auto result = ParticleEmitter::ToJson();
	result["Interval"] = myBurstInterval;
	result["IsLooping"] = myIsLooping;
	result["IsActive"] = myIsActive;
	return result;
}

void BurstEmitter::CreateParticles()
{
	float multiplier = 1.f;
	if (myBurstInterval > 0.f)
	{
		multiplier = std::ceil(myData.LifeTime / myBurstInterval);
	}
	unsigned amount = static_cast<unsigned>(std::ceil(myData.SpawnRate * Crimson::Max(multiplier, 1.f)));
	ParticleEmitter::CreateParticles(amount);
}

void BurstEmitter::UpdateParticles(float aDeltaTime)
{
	const float gravityPull = GetGravity(aDeltaTime);

	float currentLerp = 0.f;
	float speed = myData.StartSpeed * aDeltaTime;
	Crimson::Vector4f color = myData.StartColor;
	Crimson::Vector3f size = myData.StartSize;

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

			if (lerpValue != currentLerp)
			{
				currentLerp = lerpValue;
				speed = Crimson::Lerp(myData.StartSpeed, myData.EndSpeed, lerpValue) * aDeltaTime;
				color = Crimson::Vector4f::Lerp(myData.StartColor, myData.EndColor, lerpValue);
				size = Crimson::Vector3f::Lerp(myData.StartSize, myData.EndSize, lerpValue);
			}

			particle->myColor = color;
			particle->myScale = size;

			const Crimson::Vector3f& change = particle->myDirection * speed;
			particle->myPosition += change;
			particle->myPosition.y -= gravityPull;

			iter++;
		}
	}
}

void BurstEmitter::CreateBurst()
{
	const float angleIncrease = 360.f / myData.SpawnRate;
	float angle = 0.f;

	const int count = static_cast<int>(std::roundf(myData.SpawnRate));
	auto iter = myInactiveParticles.begin();
	for (int i = 0; i < count && iter != myInactiveParticles.end(); i++)
	{
		// The pointer is necessary for the loop to work correctly, it will crash when activating the last particle otherwise
		ParticleVertex* particle = *iter;
		iter = ActivateParticle(*iter);
		particle->myDirection = Crimson::Matrix3x3f::CreateRotationAroundY(angle) * Crimson::Vector3f::Forward;
		angle += angleIncrease;
	}
}
