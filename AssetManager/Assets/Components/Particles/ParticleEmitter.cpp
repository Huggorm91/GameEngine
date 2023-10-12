#include "AssetManager.pch.h"
#include "ParticleEmitter.h"
#include "Json/jsonCpp/json.h"

ParticleEmitter::ParticleEmitter(unsigned anID): myID(anID), myTransform()
{
}

void ParticleEmitter::Init(const EmitterData*)
{
}

Json::Value ParticleEmitter::ToJson() const
{
	Json::Value result;
	result["ID"] = myID;
	result["Transform"] = myTransform.ToJson();
	return result;
}
