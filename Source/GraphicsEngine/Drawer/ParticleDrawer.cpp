#include "GraphicsEngine.pch.h"
#include "ParticleDrawer.h"

unsigned ParticleDrawer::GetNewEmitterID() const
{
    static unsigned localEmitterIDCount = 0;
    return ++localEmitterIDCount;
}
