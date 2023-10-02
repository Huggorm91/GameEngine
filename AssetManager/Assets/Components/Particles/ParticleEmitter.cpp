#include "AssetManager.pch.h"
#include "ParticleEmitter.h"
#include "External/jsonCpp/json.h"

ParticleEmitter::ParticleEmitter(unsigned anID): myID(anID), myTransform()
{
}

void ParticleEmitter::Init(const EmitterData* someData)
{
}

Json::Value ParticleEmitter::ToJson() const
{
	Json::Value result;
	result["ID"] = myID;
	result["Transform"] = myTransform.ToJson();
	return result;
}
