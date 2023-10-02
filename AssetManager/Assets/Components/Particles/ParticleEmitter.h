#pragma once
#include "../Transform.h"
#include "EmitterData.h"

class ParticleEmitter
{
public:
	enum class EmitterType
	{
		Unknown
	};
	ParticleEmitter(unsigned anID);
	ParticleEmitter(const Json::Value& aJson);
	~ParticleEmitter() = default;

	void Init(const EmitterData* someData);

	Json::Value ToJson() const;

private:
	unsigned myID;
	Transform myTransform;	
};