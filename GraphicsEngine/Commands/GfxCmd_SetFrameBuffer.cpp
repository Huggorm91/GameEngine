#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetFrameBuffer.h"
#include <Timer.h>

GfxCmd_SetFrameBuffer::GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4f& aViewMatrix, const CommonUtilities::Matrix4x4f& aProjectionMatrix, const CommonUtilities::Vector3f& aCameraPosition) :
	GraphicsCommand(RenderStage::Deferred), myViewMatrix(aViewMatrix), myProjectionMatrix(aProjectionMatrix), myCameraPosition(aCameraPosition)
{
}

void GfxCmd_SetFrameBuffer::Execute()
{
	FrameBuffer& buffer = GetFrameBuffer();
	buffer.Data.View = myViewMatrix;
	buffer.Data.Projection = myProjectionMatrix;
	buffer.Data.CameraPosition = myCameraPosition;
	buffer.Data.DeltaTime = CommonUtilities::Timer::GetDeltaTime();
	buffer.Data.TotalTime = CommonUtilities::Timer::GetTotalTime();

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 0, buffer);
}