#pragma once
#include "Math/Matrix4x4.hpp"

class LineHandle
{
public:
	LineHandle();
	LineHandle(const LineHandle& aHandle);
	~LineHandle() = default;
	LineHandle& operator=(const LineHandle& aHandle);
	LineHandle& operator=(LineHandle&& aHandle);

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