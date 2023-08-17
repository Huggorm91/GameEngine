#pragma once
#include "../Rendering/LineVertex.h"
#include "../Rendering/Shader.h"
#include <Matrix4x4.hpp>
#include <unordered_set>

class LineDrawer
{
public:
	class LineHandle
	{
	public:
		LineHandle();
		LineHandle(const LineHandle& aHandle);
		~LineHandle() = default;
		LineHandle& operator=(const LineHandle& aHandle);

		bool IsValid() const;
		bool IsActive() const;

		void ToogleActive() const;
		void SetActive(bool aValue) const;

		void UpdateTransform(const CommonUtilities::Matrix4x4f& aTransform) const;

	private:
		const unsigned myID;

		friend class LineDrawer;
		LineHandle(unsigned anID);
	};

	LineDrawer() = default;
	~LineDrawer() = default;

	bool Init();
	void Render();

	LineHandle AddLine(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddArrow(const CommonUtilities::Vector3f& aFrom, const CommonUtilities::Vector3f& aTo, float aHeadSize, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	LineHandle AddAxisLines(const CommonUtilities::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const CommonUtilities::Vector3f& aXDirection = { 1.f, 0.f, 0.f }, const CommonUtilities::Vector3f& aYDirection = { 0.f, 1.f, 0.f }, const CommonUtilities::Vector3f& aZDirection = { 0.f, 0.f, 1.f }, const CommonUtilities::Vector4f& aXColor = GetColor(eColor::Red), const CommonUtilities::Vector4f& aYColor = GetColor(eColor::Green), const CommonUtilities::Vector4f& aZColor = GetColor(eColor::Blue), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSquare(const CommonUtilities::Vector2f& aMin, const CommonUtilities::Vector2f& aMax, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSquare(const CommonUtilities::Vector2f& aCenter, const CommonUtilities::Vector2f& anExtent, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCircle(const CommonUtilities::Vector2f& aCenter, float aRadius, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddTriangle(const CommonUtilities::Vector2f& aFrom, const CommonUtilities::Vector2f& aTo, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform =CommonUtilities::Matrix4x4f::Null}, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCube(const CommonUtilities::Vector3f& aMin, const CommonUtilities::Vector3f& aMax, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCube(const CommonUtilities::Vector3f& aCenter, const CommonUtilities::Vector3f& anExtent, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSphere(const CommonUtilities::Vector3f& aCenter, float aRadius, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCylinder(const CommonUtilities::Vector3f& aCenter, float aHeight, float aBaseRadius, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCapsule(const CommonUtilities::Vector3f& aCenter, float aHeight, float aBaseRadius, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCone(float aHeight, float aBaseRadius, const CommonUtilities::Vector4f& aColor = GetColor(eColor::White), const CommonUtilities::Matrix4x4f& aTransform = CommonUtilities::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);

	LineHandle GetNewHandle();
	void UpdatePrimitiveTransform(const LineHandle& aHandle, const CommonUtilities::Matrix4x4f& aTransform);
	void ActivateHandle(const LineHandle& aHandle);
	void DeactivateHandle(const LineHandle& aHandle);
	void DeleteHandle(const LineHandle& aHandle);

	// True if handle exists in LineDrawer
	bool IsValid(const LineHandle& aHandle) const;
	// True if handle is actively being drawn by LineDrawer
	bool IsActive(const LineHandle& aHandle) const;

private:
	struct LinePrimitive
	{
		CommonUtilities::Matrix4x4f myTransform;
		std::vector<LineVertex> myVertices;
		std::vector<unsigned> myIndices;
		bool myIsUI;
	};

	std::unordered_map<unsigned, LinePrimitive> myLines;
	std::unordered_set<unsigned> myActiveLines;
	std::unordered_set<unsigned> myActiveUILines;

	Shader myVertexShader;
	Shader myUIVertexShader;
	Shader myPixelShader;

	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	ComPtr<ID3D11Buffer> myUIVertexBuffer;
	ComPtr<ID3D11Buffer> myUIIndexBuffer;
	size_t myIndexCount;
	size_t myUIIndexCount;

	unsigned myCounter;
	bool myIsDirty;
	bool myUIIsDirty;

	unsigned AddPrimitive(const LinePrimitive& aPrimitive);
	void UpdatePrimitive(const LinePrimitive& aPrimitive, const LineHandle& aHandle);

	void UpdateBuffers(bool aIsUI);
};