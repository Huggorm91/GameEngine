#pragma once
#include "GfxCmd_RenderMesh.h"
#include "GfxCmd_RenderMeshShadow.h"
#include "Light/LightCommand.h"

struct CommandContainer
{
	std::vector<std::shared_ptr<GraphicsCommand>> lightRenderCommands{};
	std::vector<std::shared_ptr<LightCommand>> lightCommands{};
	std::vector<std::shared_ptr<GraphicsCommand>> shadowRenderCommands{};
	std::vector<std::shared_ptr<GfxCmd_RenderMeshShadow>> shadowCommands{};
	std::vector<std::shared_ptr<GraphicsCommand>> deferredRenderCommands{};
	std::vector<std::shared_ptr<GfxCmd_RenderMesh>> deferredMeshCommands{};
	std::vector<std::shared_ptr<GraphicsCommand>> forwardRenderCommands{};
	std::vector<std::shared_ptr<GfxCmd_RenderMesh>> forwardMeshCommands{};
	std::vector<std::shared_ptr<GraphicsCommand>> postProcessRenderCommands{};
};