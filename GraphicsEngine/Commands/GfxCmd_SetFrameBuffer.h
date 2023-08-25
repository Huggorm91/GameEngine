#pragma once
#include "GraphicsCommand.h"

class GfxCmd_SetFrameBuffer : public GraphicsCommand
{
public:
	GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4f& aViewMatrix, const CommonUtilities::Matrix4x4f& aProjectionMatrix, const CommonUtilities::Vector3f& aCameraPosition);
	~GfxCmd_SetFrameBuffer() override = default;

	void Execute() override;

private:
	CommonUtilities::Matrix4x4f myViewMatrix;
	CommonUtilities::Matrix4x4f myProjectionMatrix;
	CommonUtilities::Vector3f myCameraPosition;
};