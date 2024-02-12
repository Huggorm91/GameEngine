#pragma once
#include "Rendering/ParticleVertex.h"
#include "Rendering/Shader.h"

class ParticleEmitter;

class ParticleDrawer
{
public:
	ParticleDrawer() = default;
	~ParticleDrawer() = default;

	void Render();

	void AddEmitter(ParticleEmitter* anEmitter);
	void RemoveEmitter(ParticleEmitter* anEmitter);

private:
	std::unordered_set<ParticleEmitter*> myEmitters;

	friend class ParticleEmitter;
	unsigned GetNewEmitterID() const;
};