#include "GraphicsEngine.pch.h"
#include "LineDrawer.h"
#include "../GraphicsEngine.h"
#include "AssetManager/AssetManager.h"

LineDrawer::LineHandle::LineHandle(unsigned anID) : myID(anID)
{
}

LineDrawer::LineHandle::LineHandle() : myID(UINT_MAX)
{
}

LineDrawer::LineHandle::LineHandle(const LineHandle& aHandle) : myID(aHandle.myID)
{
}

LineDrawer::LineHandle& LineDrawer::LineHandle::operator=(const LineHandle& aHandle)
{
	const_cast<unsigned&>(myID) = aHandle.myID;
	return *this;
}

bool LineDrawer::LineHandle::IsValid() const
{
	return GraphicsEngine::Get().GetLineDrawer().IsValid(*this);
}

bool LineDrawer::LineHandle::IsActive() const
{
	return GraphicsEngine::Get().GetLineDrawer().IsActive(*this);
}

void LineDrawer::LineHandle::ToogleActive() const
{
	if (IsActive())
	{
		GraphicsEngine::Get().GetLineDrawer().DeactivateHandle(*this);
	}
	else
	{
		GraphicsEngine::Get().GetLineDrawer().ActivateHandle(*this);
	}
}

void LineDrawer::LineHandle::SetActive(bool aValue) const
{
	if (aValue)
	{
		GraphicsEngine::Get().GetLineDrawer().ActivateHandle(*this);
	}
	else
	{
		GraphicsEngine::Get().GetLineDrawer().DeactivateHandle(*this);
	}
}

void LineDrawer::LineHandle::UpdateTransform(const CommonUtilities::Matrix4x4f& aTransform) const
{
	GraphicsEngine::Get().GetLineDrawer().UpdatePrimitiveTransform(*this, aTransform);
}

LineDrawer::LineHandle LineDrawer::GetNewHandle()
{
	myLines.emplace(myCounter, LinePrimitive());
	return myCounter++;
}

void LineDrawer::UpdatePrimitiveTransform(const LineHandle& aHandle, const CommonUtilities::Matrix4x4f& aTransform)
{
	if (auto iter = myLines.find(aHandle.myID); iter != myLines.end())
	{
		if (iter->second.myIsUI)
		{
			myUIIsDirty = true;
		}
		else
		{
			myIsDirty = true;
		}
		iter->second.myTransform = aTransform;
	}
}

void LineDrawer::ActivateHandle(const LineHandle& aHandle)
{
	if (auto iter = myLines.find(aHandle.myID); iter != myLines.end())
	{
		if (iter->second.myIsUI)
		{
			myUIIsDirty = true;
			myActiveUILines.emplace(aHandle.myID);
		}
		else
		{
			myIsDirty = true;
			myActiveLines.emplace(aHandle.myID);
		}
	}
}

void LineDrawer::DeactivateHandle(const LineHandle& aHandle)
{
	if (auto iter = myActiveLines.find(aHandle.myID); iter != myActiveLines.end())
	{
		myIsDirty = true;
		myActiveLines.erase(iter);
	}
	else if (auto iter = myActiveUILines.find(aHandle.myID); iter != myActiveUILines.end())
	{
		myUIIsDirty = true;
		myActiveUILines.erase(iter);
	}
}

void LineDrawer::DeleteHandle(const LineHandle& aHandle)
{
	if (auto iter = myLines.find(aHandle.myID); iter != myLines.end())
	{
		DeactivateHandle(aHandle);
		myLines.erase(iter);
	}
}

bool LineDrawer::IsValid(const LineHandle& aHandle) const
{
	return myLines.find(aHandle.myID) != myLines.end();
}

bool LineDrawer::IsActive(const LineHandle& aHandle) const
{
	if (myActiveLines.find(aHandle.myID) != myActiveLines.end())
	{
		return true;
	}
	else
	{
		return myActiveUILines.find(aHandle.myID) != myActiveUILines.end();
	}
}

bool LineDrawer::Init()
{
	myVertexShader = AssetManager::GetAsset<Shader&>("Content/Shaders/Default_Line_VS.cso");
	myUIVertexShader = AssetManager::GetAsset<Shader&>("Content/Shaders/Default_UI_Line_VS.cso");
	myPixelShader = AssetManager::GetAsset<Shader&>("Content/Shaders/Default_Line_PS.cso");
	if (!RHI::CreateInputLayout(LineVertex::InputLayout, LineVertex::InputLayoutDefinition, myVertexShader.GetBlob(), myVertexShader.GetBlobSize()))
	{
		GELogger.Err("LineDrawer: Failed to create InputLayout!");
		return false;
	}
	if (!RHI::CreateDynamicVertexBuffer(myVertexBuffer, 65536, sizeof(LineVertex)) || !RHI::CreateDynamicIndexBuffer(myIndexBuffer, 65536))
	{
		GELogger.Err("LineDrawer: Failed to create buffers!");
		return false;
	}
	if (!RHI::CreateDynamicVertexBuffer(myUIVertexBuffer, 65536, sizeof(LineVertex)) || !RHI::CreateDynamicIndexBuffer(myUIIndexBuffer, 65536))
	{
		GELogger.Err("LineDrawer: Failed to create UI buffers!");
		return false;
	}
	return true;
}

void LineDrawer::Render()
{
	if (myIsDirty)
	{
		UpdateBuffers(false);
		myIsDirty = false;
	}

	if (myUIIsDirty)
	{
		UpdateBuffers(true);
		myUIIsDirty = false;
	}

	if (myIndexCount > 0)
	{
		RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, myVertexBuffer, myIndexBuffer, sizeof(LineVertex), LineVertex::InputLayout);

		RHI::SetVertexShader(&myVertexShader);
		RHI::SetPixelShader(&myPixelShader);
		RHI::DrawIndexed(myIndexCount);

		RHI::SetVertexShader(nullptr);
		RHI::SetPixelShader(nullptr);
	}

	if (myUIIndexCount > 0)
	{
		RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, myUIVertexBuffer, myUIIndexBuffer, sizeof(LineVertex), LineVertex::InputLayout);

		RHI::SetVertexShader(&myUIVertexShader);
		RHI::SetPixelShader(&myPixelShader);
		RHI::DrawIndexed(myUIIndexCount);

		RHI::SetVertexShader(nullptr);
		RHI::SetPixelShader(nullptr);
	}
}

LineDrawer::LineHandle LineDrawer::AddLine(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, const CommonUtilities::Vector4f& aColor, const CommonUtilities::Matrix4x4f& aTransform, bool aIsUI, LineHandle* aHandle)
{
	LinePrimitive primitive;
	primitive.myIsUI = aIsUI;
	primitive.myTransform = aTransform;
	primitive.myVertices.emplace_back(aFrom, aColor);
	primitive.myVertices.emplace_back(aTo, aColor);
	primitive.myIndices.emplace_back(0);
	primitive.myIndices.emplace_back(1);

	if (aHandle)
	{
		UpdatePrimitive(primitive, *aHandle);
		return aHandle->myID;
	}
	else
	{
		return LineHandle(AddPrimitive(primitive));
	}
}

LineDrawer::LineHandle LineDrawer::AddAxisLines(const CommonUtilities::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const CommonUtilities::Vector3f& aXDirection, const CommonUtilities::Vector3f& aYDirection, const CommonUtilities::Vector3f& aZDirection, const CommonUtilities::Vector4f& aXColor, const CommonUtilities::Vector4f& aYColor, const CommonUtilities::Vector4f& aZColor, const CommonUtilities::Matrix4x4f& aTransform, bool aIsUI, LineHandle* aHandle)
{
	LinePrimitive primitive;
	primitive.myIsUI = aIsUI;
	primitive.myTransform = aTransform;
	if (aIsAxisPointingBothWays)
	{
		primitive.myVertices.emplace_back(aCenter - (aXDirection.GetNormalized() * aLineLength), aXColor);
		primitive.myVertices.emplace_back(aCenter + (aXDirection.GetNormalized() * aLineLength), aXColor);
		primitive.myVertices.emplace_back(aCenter - (aYDirection.GetNormalized() * aLineLength), aYColor);
		primitive.myVertices.emplace_back(aCenter + (aYDirection.GetNormalized() * aLineLength), aYColor);
		primitive.myVertices.emplace_back(aCenter - (aZDirection.GetNormalized() * aLineLength), aZColor);
		primitive.myVertices.emplace_back(aCenter + (aZDirection.GetNormalized() * aLineLength), aZColor);
	}
	else
	{
		primitive.myVertices.emplace_back(aCenter, aXColor);
		primitive.myVertices.emplace_back(aCenter + (aXDirection.GetNormalized() * aLineLength), aXColor);
		primitive.myVertices.emplace_back(aCenter, aYColor);
		primitive.myVertices.emplace_back(aCenter + (aYDirection.GetNormalized() * aLineLength), aYColor);
		primitive.myVertices.emplace_back(aCenter, aZColor);
		primitive.myVertices.emplace_back(aCenter + (aZDirection.GetNormalized() * aLineLength), aZColor);
	}
	primitive.myIndices.emplace_back(0);
	primitive.myIndices.emplace_back(1);
	primitive.myIndices.emplace_back(2);
	primitive.myIndices.emplace_back(3);
	primitive.myIndices.emplace_back(4);
	primitive.myIndices.emplace_back(5);

	if (aHandle)
	{
		UpdatePrimitive(primitive, *aHandle);
		return aHandle->myID;
	}
	else
	{
		return LineHandle(AddPrimitive(primitive));
	}
}

unsigned LineDrawer::AddPrimitive(const LinePrimitive& aPrimitive)
{
	myLines.emplace(myCounter, aPrimitive);
	if (aPrimitive.myIsUI)
	{
		myUIIsDirty = true;
		myActiveUILines.emplace(myCounter);
	}
	else
	{
		myIsDirty = true;
		myActiveLines.emplace(myCounter);
	}

	return myCounter++;
}

void LineDrawer::UpdatePrimitive(const LinePrimitive& aPrimitive, const LineHandle& aHandle)
{
	auto& primitive = myLines.at(aHandle.myID);
	bool isActive = IsActive(aHandle);

	if (isActive && primitive.myIsUI != aPrimitive.myIsUI)
	{
		myUIIsDirty = true;
		myIsDirty = true;
		DeactivateHandle(aHandle);

		if (aPrimitive.myIsUI)
		{
			myActiveUILines.emplace(myCounter);
		}
		else
		{
			myActiveLines.emplace(myCounter);
		}
	}
	else if (isActive)
	{
		if (aPrimitive.myIsUI)
		{
			myUIIsDirty = true;
		}
		else
		{
			myIsDirty = true;
		}
	}

	primitive = aPrimitive;
}

void LineDrawer::UpdateBuffers(bool aIsUI)
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	std::unordered_set<unsigned>* activeLines;
	size_t* indexCount;

	if (aIsUI)
	{
		vertexBuffer = myUIVertexBuffer.Get();
		indexBuffer = myUIIndexBuffer.Get();
		activeLines = &myActiveUILines;
		indexCount = &myUIIndexCount;
	}
	else
	{
		vertexBuffer = myVertexBuffer.Get();
		indexBuffer = myIndexBuffer.Get();
		activeLines = &myActiveLines;
		indexCount = &myIndexCount;
	}

	D3D11_MAPPED_SUBRESOURCE vertexResource{};
	D3D11_MAPPED_SUBRESOURCE indexResource{};

	RHI::Context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource);
	RHI::Context->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource);

	unsigned currentVertexOffset = 0;
	unsigned currentIndexOffset = 0;

	for (auto& index : *activeLines)
	{
		const LinePrimitive& currentPrimitive = myLines.at(index);
		LineVertex* vertexPointer = static_cast<LineVertex*>(vertexResource.pData) + currentVertexOffset;

		for (size_t v = 0; v < currentPrimitive.myVertices.size(); v++)
		{
			LineVertex& currentVertex = vertexPointer[v];
			const LineVertex& primitiveVertex = currentPrimitive.myVertices[v];
			currentVertex = primitiveVertex;
			currentVertex.myPosition = currentPrimitive.myTransform * primitiveVertex.myPosition;
		}

		unsigned* indexPointer = static_cast<unsigned*>(indexResource.pData) + currentIndexOffset;

		for (size_t i = 0; i < currentPrimitive.myIndices.size(); i++)
		{
			indexPointer[i] = currentPrimitive.myIndices[i] + currentVertexOffset;
		}

		currentVertexOffset += static_cast<unsigned>(currentPrimitive.myVertices.size());
		currentIndexOffset += static_cast<unsigned>(currentPrimitive.myIndices.size());
	}

	*indexCount = currentIndexOffset;

	RHI::Context->Unmap(vertexBuffer, 0);
	RHI::Context->Unmap(indexBuffer, 0);
}
