#pragma once
#include "Rendering/ParticleVertex.h"
#include "Rendering/Shader.h"

class ParticleEmitter;

class ParticleDrawer
{
public:
	ParticleDrawer() = default;
	~ParticleDrawer() = default;

	bool Init();

	void Render();

	void AddEmitter(const ParticleEmitter* anEmitter);
	void RemoveEmitter(const ParticleEmitter* anEmitter);

private:
	std::unordered_set<const ParticleEmitter*> myEmitters;
};