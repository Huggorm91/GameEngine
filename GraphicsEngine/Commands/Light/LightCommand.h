#pragma once
#include "Rendering/Buffers/LightBuffer.h"

class LightCommand 
{
public:
	enum class Type
	{
		Other,
		PointLight,
		SpotLight,
		Directional
	};
	LightCommand();
	virtual ~LightCommand() = default;

	virtual void Execute(const int anIndex) = 0;
	virtual void SetShadowMap(const int anIndex) = 0;

	Type GetType();

protected:
	Type myType;

	LightCommand(Type aType);
	LightBuffer& GetLightBuffer();
	const Texture* GetDefaultCubemap();
};