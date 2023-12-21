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
	void Render(float aGravitationalPull);

	void AddEmitter(ParticleEmitter& anEmitter);
	bool RemoveEmitter(const ParticleEmitter& anEmitter);

private:
	std::unordered_map<unsigned, ParticleEmitter*> myEmitters;

	Shader myVertexShader;
	Shader myPixelShader;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;

	size_t myIndexCount;

	friend class ParticleEmitter;
	unsigned GetNewEmitterID() const;
};