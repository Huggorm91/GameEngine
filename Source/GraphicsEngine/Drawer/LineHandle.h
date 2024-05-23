#pragma once
#include "CrimsonUtilities/Math/Matrix4x4.hpp"

class LineHandle
{
public:
	LineHandle();
	LineHandle(const LineHandle& aHandle);
	LineHandle(LineHandle&& aHandle) noexcept;
	~LineHandle() = default;
	LineHandle& operator=(const LineHandle& aHandle);
	LineHandle& operator=(LineHandle&& aHandle) noexcept;

	bool IsValid() const;
	bool IsActive() const;

	void ToogleActive() const;
	void SetActive(bool aValue) const;

	void UpdateTransform(const Crimson::Matrix4x4f& aTransform) const;
	void UpdateColor(const Crimson::Vector4f& aColor) const;

	void Delete() const;

private:
	const unsigned myID;

	friend class LineDrawer;
	LineHandle(unsigned anID);
};