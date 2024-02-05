#include "AssetManager.pch.h"
#include "ParticleEmitter.h"
#include "GraphicsEngine/GraphicsEngine.h"

ParticleEmitter::ParticleEmitter(): myID(GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID()), myData(nullptr), myTransform()
{
}

ParticleEmitter::ParticleEmitter(const Json::Value& aJson): myID(aJson["ID"].asUInt()), myData(nullptr), myTransform(aJson["Transform"])
{
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& anEmitter): myID(GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID()), myData(anEmitter.myData), myTransform(anEmitter.myTransform)
{
}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& anEmitter) : myID(anEmitter.myID), myData(anEmitter.myData), myTransform(anEmitter.myTransform)
{
}

ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& anEmitter)
{
	myID=GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID();
	myData=anEmitter.myData;
	myTransform=anEmitter.myTransform;
	return *this;
}

void ParticleEmitter::Init(const EmitterData* someData)
{
	myData = someData;
}

Json::Value ParticleEmitter::ToJson() const
{
	Json::Value result;
	result["ID"] = myID;
	result["Transform"] = myTransform.ToJson();
	return result;
}
