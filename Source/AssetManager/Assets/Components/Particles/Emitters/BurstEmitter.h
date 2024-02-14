#pragma once
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

class BurstEmitter : public ParticleEmitter
{
public:
	BurstEmitter();
	BurstEmitter(const Json::Value& aJson);
	BurstEmitter(const BurstEmitter& anEmitter) = default;
	BurstEmitter(BurstEmitter&& anEmitter) noexcept = default;
	virtual ~BurstEmitter() = default;
	BurstEmitter& operator=(const BurstEmitter& anEmitter) = default;
	BurstEmitter& operator=(BurstEmitter&& anEmitter) noexcept = default;

	void Update(float aDeltaTime) override;

	void CreateImGuiElements() override;
	Json::Value ToJson() const override;

private:
	float myBurstInterval;
	float myTimer;
	bool myIsLooping;
	bool myIsActive;

	void UpdateParticles(float aDeltaTime);
	void CreateBurst();
};