#include "GraphicsEngine.pch.h"
#include "ParticleDrawer.h"
#include "ParticleEmitter.h"
#include "GraphicsEngine.h"

bool ParticleDrawer::Init()
{
    return RHI::CreateInputLayout(ParticleVertex::InputLayout, ParticleVertex::InputLayoutDefinition, GraphicsEngine::Get().GetDefaultParticleVS()->GetBlob(), GraphicsEngine::Get().GetDefaultParticleVS()->GetBlobSize());
}

void ParticleDrawer::Render()
{
    for (auto& emitter : myEmitters)
    {
        emitter->SetAsResource();
        emitter->Render();
    }
    myEmitters.clear();
}

void ParticleDrawer::AddEmitter(const ParticleEmitter* anEmitter)
{
    myEmitters.emplace(anEmitter);
}

void ParticleDrawer::RemoveEmitter(const ParticleEmitter* anEmitter)
{
    if (auto iter = myEmitters.find(anEmitter); iter != myEmitters.end())
    {
        myEmitters.erase(iter);
    }
}
