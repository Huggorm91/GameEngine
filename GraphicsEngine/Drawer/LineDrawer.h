#pragma once
#include "../Rendering/LineVertex.h"
#include "../Rendering/Shader.h"
#include "Math/Matrix4x4.hpp"
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

		void UpdateTransform(const Crimson::Matrix4x4f& aTransform) const;

	private:
		const unsigned myID;

		friend class LineDrawer;
		LineHandle(unsigned anID);
	};

	LineDrawer() = default;
	~LineDrawer() = default;

	bool Init();
	void Render();

	LineHandle AddLine(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, const Crimson::Vector4f& aColor = ColorManager::GetColor("White"), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddArrow(const Crimson::Vector3f& aFrom, const Crimson::Vector3f& aTo, float aHeadSize, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	LineHandle AddAxisLines(const Crimson::Vector3f& aCenter, float aLineLength, bool aIsAxisPointingBothWays, const Crimson::Vector3f& aXDirection = { 1.f, 0.f, 0.f }, const Crimson::Vector3f& aYDirection = { 0.f, 1.f, 0.f }, const Crimson::Vector3f& aZDirection = { 0.f, 0.f, 1.f }, const Crimson::Vector4f& aXColor = ColorManager::GetColor("Red"), const Crimson::Vector4f& aYColor = ColorManager::GetColor("Green"), const Crimson::Vector4f& aZColor = ColorManager::GetColor("Blue"), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSquare(const Crimson::Vector2f& aMin, const Crimson::Vector2f& aMax, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSquare(const Crimson::Vector2f& aCenter, const Crimson::Vector2f& anExtent, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCircle(const Crimson::Vector2f& aCenter, float aRadius, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddTriangle(const Crimson::Vector2f& aFrom, const Crimson::Vector2f& aTo, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform =Crimson::Matrix4x4f::Null}, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCube(const Crimson::Vector3f& aMin, const Crimson::Vector3f& aMax, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCube(const Crimson::Vector3f& aCenter, const Crimson::Vector3f& anExtent, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddSphere(const Crimson::Vector3f& aCenter, float aRadius, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCylinder(const Crimson::Vector3f& aCenter, float aHeight, float aBaseRadius, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCapsule(const Crimson::Vector3f& aCenter, float aHeight, float aBaseRadius, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCone(float aHeight, float aBaseRadius, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);
	//LineHandle AddCone(float aHeight, float anAngleInRadians, const Crimson::Vector3f& aPosition, const Crimson::Vector3f& aDirection, const Crimson::Vector4f& aColor = GetColor(eColor::White), const Crimson::Matrix4x4f& aTransform = Crimson::Matrix4x4f::Null, bool aIsUI = false, LineHandle* aHandle = nullptr);

	LineHandle GetNewHandle();
	void UpdatePrimitiveTransform(const LineHandle& aHandle, const Crimson::Matrix4x4f& aTransform);
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
		Crimson::Matrix4x4f myTransform{};
		std::vector<LineVertex> myVertices{};
		std::vector<unsigned> myIndices{};
		bool myIsUI{};
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

	void CreateCircle(LinePrimitive& outPrimitive, unsigned aVertexCount, float aRadius, const Crimson::Vector3f& aCenter, const Crimson::Vector3f& aDirection, const Crimson::Vector4f& aColor);

	void UpdateBuffers(bool aIsUI);
};