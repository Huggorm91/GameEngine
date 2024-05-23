#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetFrameBuffer.h"
#include "CrimsonUtilities/Time/Timer.h"

GfxCmd_SetFrameBuffer::GfxCmd_SetFrameBuffer(const Crimson::Matrix4x4f& aViewMatrix, const Crimson::Matrix4x4f& aProjectionMatrix, const Crimson::Vector3f& aCameraPosition) :
	GraphicsCommand(RenderStage::Deferred), myViewMatrix(aViewMatrix), myProjectionMatrix(aProjectionMatrix), myCameraPosition(aCameraPosition)
{
}

void GfxCmd_SetFrameBuffer::Execute()
{
	FrameBuffer& buffer = GetFrameBuffer();
	buffer.Data.View = myViewMatrix;
	buffer.Data.Projection = myProjectionMatrix;
	buffer.Data.CameraPosition = myCameraPosition;
	buffer.Data.DeltaTime = Crimson::Timer::GetDeltaTime();
	buffer.Data.TotalTime = Crimson::Timer::GetTotalTime();

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_GEOMETERY_SHADER | PIPELINE_STAGE_PIXEL_SHADER, GetFrameBufferSlot(), buffer);
}