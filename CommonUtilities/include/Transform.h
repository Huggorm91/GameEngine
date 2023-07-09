#pragma once
#include <Matrix4x4.hpp>

namespace CommonUtilities
{
	class Transform
	{
	public:
		Transform();
		Transform(const CommonUtilities::Vector3<float>& aPosition, const CommonUtilities::Vector3<float>& aRotation = {}, const CommonUtilities::Vector3<float>& aScale = { 1.f, 1.f, 1.f });
		~Transform() = default;

		void SetPosition(const CommonUtilities::Vector3<float>& aPosition);
		const CommonUtilities::Vector3<float>& GetPosition() const;

		void SetRotation(const CommonUtilities::Vector3<float>& aRotation);
		const CommonUtilities::Vector3<float>& GetRotation() const;

		void SetScale(const CommonUtilities::Vector3<float>& aScale);
		const CommonUtilities::Vector3<float>& GetScale() const;

		const CommonUtilities::Matrix4x4<float>& GetTransform() const;

		bool HasChanged() const;

	private:
		bool myHasChanged;
		CommonUtilities::Vector3<float> myPosition;
		CommonUtilities::Vector3<float> myRotation;
		CommonUtilities::Vector3<float> myScale;

		CommonUtilities::Matrix4x4<float> myTransform;

		void UpdateTransform();
	};
}