#include "GraphicsEngine.pch.h"
#include "LineVertex.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC> LineVertex::InputLayoutDefinition =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ComPtr<ID3D11InputLayout> LineVertex::InputLayout;

LineVertex::LineVertex(): myPosition(0.f, 0.f, 0.f, 1.f), myColor()
{
}

LineVertex::LineVertex(float aX, float aY, float aZ, float aR, float aG, float aB): myPosition(aX, aY, aZ, 1.f), myColor(aR, aG, aB, 1.f)
{
}

LineVertex::LineVertex(Crimson::Vector3f aPosition, Crimson::Vector4f aColor) : myPosition(aPosition, 1.f), myColor(aColor)
{
}
