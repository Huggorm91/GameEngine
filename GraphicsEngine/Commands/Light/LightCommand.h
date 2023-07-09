#pragma once
#include "Rendering/Buffers/LightBuffer.h"

class LightCommand 
{
public:
	enum class Type
	{
		PointLight,
		SpotLight,
		Other
	};
	LightCommand();
	virtual ~LightCommand() = default;
	virtual void Execute(const int anIndex) = 0;

	Type GetType();

protected:
	Type myType;

	LightCommand(Type aType);
	LightBuffer& GetLightBuffer();
};