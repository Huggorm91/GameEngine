#pragma once
#include "../Transform.h"
#include "GraphicsEngine/Drawer/EmitterData.h"

class ParticleEmitter
{
public:
	enum class EmitterType
	{
		Unknown,
		Burst,
		Stream
	};

	ParticleEmitter();
	ParticleEmitter(const Json::Value& aJson);
	ParticleEmitter(const ParticleEmitter& anEmitter);
	ParticleEmitter(ParticleEmitter&& anEmitter);
	virtual ~ParticleEmitter() = default;
	ParticleEmitter& operator=(const ParticleEmitter& anEmitter);

	virtual void Init(const EmitterData* someData);

	//virtual void Start() = 0;
	//virtual void Stop() = 0;

	virtual Json::Value ToJson() const;

protected:
	unsigned myID;
	const EmitterData* myData;
	Transform myTransform;	
};