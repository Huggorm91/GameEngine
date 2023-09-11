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

	virtual Texture* GetShadowMap();
	virtual void SetShadowMap(const int anIndex);
	virtual bool CastsShadow() const;

	Type GetType();

protected:
	Type myType;

	LightCommand(Type aType);
	LightBuffer& GetLightBuffer();
	const Texture* GetDefaultCubemap();
	Texture*& GetDirectionalShadowMap();
	std::array<Texture*, MAX_LIGHTS>& GetPointlightShadowMap();
	std::array<Texture*, MAX_LIGHTS>& GetSpotlightShadowMap();
};