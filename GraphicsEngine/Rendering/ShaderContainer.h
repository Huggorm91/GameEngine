#pragma once
#include "Shader.h"

struct ShaderContainer
{
	Shader QuadVSShader{};
	Shader GBufferPSShader{};
	Shader EnvironmentPSShader{};
	Shader PointlightPSShader{};
	Shader SpotlightPSShader{};
};