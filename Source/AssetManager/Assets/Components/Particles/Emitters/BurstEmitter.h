#pragma once
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

class BurstEmitter : public ParticleEmitter
{
public:
	BurstEmitter();
	BurstEmitter(const Json::Value& aJson);
	BurstEmitter(const ParticleEmitter& anEmitter);
	BurstEmitter(const BurstEmitter& anEmitter) = default;
	BurstEmitter(BurstEmitter&& anEmitter) noexcept = default;
	virtual ~BurstEmitter() = default;
	BurstEmitter& operator=(const BurstEmitter& anEmitter) = default;
	BurstEmitter& operator=(BurstEmitter&& anEmitter) noexcept = default;

	void Update(float aDeltaTime) override;
	Json::Value ToJson() const override;

private:
	float myBurstInterval;
	float myTimer;
};