#include "GraphicsEngine.pch.h"
#include "ParticleDrawer.h"
#include "ParticleEmitter.h"

void ParticleDrawer::Render()
{
    for (auto& emitter : myEmitters)
    {
        emitter->SetAsResource();
        emitter->Render();
    }
}

void ParticleDrawer::AddEmitter(ParticleEmitter* anEmitter)
{
    myEmitters.emplace(anEmitter);
}

void ParticleDrawer::RemoveEmitter(ParticleEmitter* anEmitter)
{
    if (auto iter = myEmitters.find(anEmitter); iter != myEmitters.end())
    {
        myEmitters.erase(iter);
    }
}

unsigned ParticleDrawer::GetNewEmitterID() const
{
    static unsigned localEmitterIDCount = 0;
    return ++localEmitterIDCount;
}
