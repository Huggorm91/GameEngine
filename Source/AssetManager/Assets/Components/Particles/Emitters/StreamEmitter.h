#pragma once
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

class StreamEmitter: public ParticleEmitter
{
public:
	StreamEmitter();
	StreamEmitter(const Json::Value& aJson);
	StreamEmitter(const StreamEmitter& anEmitter) = default;
	StreamEmitter(StreamEmitter&& anEmitter) noexcept = default;
	virtual ~StreamEmitter() = default;
	StreamEmitter& operator=(const StreamEmitter& anEmitter) = default;
	StreamEmitter& operator=(StreamEmitter&& anEmitter) noexcept = default;

	void Update(float aDeltaTime) override;

	void CreateImGuiElements() override;
private:
	float myTimer;
};