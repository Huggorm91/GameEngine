#pragma once
#include "GraphicsCommand.h"

class GfxCmd_SetFrameBuffer : public GraphicsCommand
{
public:
	GfxCmd_SetFrameBuffer(const Crimson::Matrix4x4f& aViewMatrix, const Crimson::Matrix4x4f& aProjectionMatrix, const Crimson::Vector3f& aCameraPosition);
	~GfxCmd_SetFrameBuffer() override = default;

	void Execute() override;

private:
	Crimson::Matrix4x4f myViewMatrix;
	Crimson::Matrix4x4f myProjectionMatrix;
	Crimson::Vector3f myCameraPosition;
};