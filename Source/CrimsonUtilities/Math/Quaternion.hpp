#pragma once
#include "Matrix3x3.hpp"

namespace Crimson
{
	template <typename T>
	class Quaternion
	{
	public:
		T w; // Real
		T x, y, z; // Imaginary

		Quaternion() = default;
		Quaternion(const T& aReal, const T& aX, const T& aY, const T& aZ);
		Quaternion(const T& aRealPart, const Vector3<T>& anImaginaryPart);
		Quaternion(const Vector3<T>& aRadianRotation);
		~Quaternion() = default;

		Vector3<T> GetAsEuler() const;
		void SetFromEuler(Vector3<T> aRadianRotation);

		Matrix3x3<T> GetAsMatrix3x3() const;
		Matrix4x4<T> GetAsMatrix4x4() const;

		Quaternion<T> GetConjugate() const;
		Quaternion<T> GetInverse() const;

		T LengthSqr() const;
		T Length() const;

		Quaternion<T>& operator*= (const Quaternion<T>& aQuaternion);
		Quaternion<T> operator*(const Quaternion<T>& aQuaternion) const;

		Quaternion<T>& operator/=(const T& aScalar);
		Quaternion<T> operator/(const T& aScalar) const;

		Quaternion<T> operator-() const;
	};

	typedef Quaternion<float> QuatF;

	template<typename T>
	inline Quaternion<T>::Quaternion(const T& aReal, const T& aX, const T& aY, const T& aZ):
		w(aReal),
		x(aX),
		y(aY),
		z(aZ)
	{}

	template<typename T>
	inline Quaternion<T>::Quaternion(const T& aRealPart, const Vector3<T>& anImaginaryPart) :
		w(aRealPart),
		x(anImaginaryPart.x),
		y(anImaginaryPart.y),
		z(anImaginaryPart.z)
	{}

	template<typename T>
	inline Quaternion<T>::Quaternion(const Vector3<T>& aRadianRotation)
	{
		SetFromEuler(aRadianRotation);
	}

	// This returns the angle in radians
	template<typename T>
	inline Vector3<T> Quaternion<T>::GetAsEuler() const
	{
		Vector3<T> result;

		// roll (x-axis rotation)
		T sinr_cosp = 2 * (w * x + y * z);
		T cosr_cosp = 1 - 2 * (x * x + y * y);
		result.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		T sinp = std::sqrt(1 + 2 * (w * y - x * z));
		T cosp = std::sqrt(1 - 2 * (w * y - x * z));
		result.y = T(2 * std::atan2(sinp, cosp) - Math::PiHalf);

		// yaw (z-axis rotation)
		T siny_cosp = 2 * (w * z + x * y);
		T cosy_cosp = 1 - 2 * (y * y + z * z);
		result.z = std::atan2(siny_cosp, cosy_cosp);

		return result;
	}

	template<typename T>
	inline void Quaternion<T>::SetFromEuler(Vector3<T> aRadianRotation)
	{
		aRadianRotation /= T(2);

		// Roll
		T cr = cos(aRadianRotation.x);
		T sr = sin(aRadianRotation.x);

		// Pitch
		T cp = cos(aRadianRotation.y);
		T sp = sin(aRadianRotation.y);

		// Yaw
		T cy = cos(aRadianRotation.z);
		T sy = sin(aRadianRotation.z);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}

	template<typename T>
	inline Quaternion<T> Quaternion<T>::GetConjugate() const
	{
		return Quaternion<T>(w, -x, -y, -z);
	}

	template<typename T>
	inline Quaternion<T> Quaternion<T>::GetInverse() const
	{
		return Quaternion<T>();
	}

	template<typename T>
	inline T Quaternion<T>::LengthSqr() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	template<typename T>
	inline T Quaternion<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template<typename T>
	inline Quaternion<T> Quaternion<T>::operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}
}