#pragma once
#include  "../HeaderFiles/Matrix4x4.hpp"

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
		Vector3<float> myPosition;
		Vector3<float> myRotation;
		Vector3<float> myScale;

		Matrix4x4<float> myTransform;

		void UpdateTransform();
	};
}