#pragma once
#include <DirectXMath.h>

namespace tnl {
	class Matrix;
	class Quaternion;
	class Vector3 final : public DirectX::XMFLOAT3 {
	public :

		Vector3() noexcept :DirectX::XMFLOAT3(0, 0, 0) {}
		Vector3(const float xx, const float yy, const float zz) noexcept : DirectX::XMFLOAT3(xx, yy, zz) {}
		explicit Vector3(const float xyz) noexcept : DirectX::XMFLOAT3(xyz, xyz, xyz) {}
		explicit Vector3(DirectX::XMVECTOR& v) noexcept { DirectX::XMStoreFloat3(this, v); }
		explicit Vector3(DirectX::XMFLOAT3& v) noexcept : DirectX::XMFLOAT3(v.x, v.y, v.z) {}

		static const Vector3 front ;
		static const Vector3 back;
		static const Vector3 left;
		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 down;
		enum class eAxis : uint32_t { FRONT, BACK, LEFT, RIGHT, UP, DOWN, MAX };
		enum class fAxis : uint32_t { 
			FRONT	= (1 << static_cast<uint32_t>(eAxis::FRONT)),
			BACK	= (1 << static_cast<uint32_t>(eAxis::BACK)),
			LEFT	= (1 << static_cast<uint32_t>(eAxis::LEFT)),
			RIGHT	= (1 << static_cast<uint32_t>(eAxis::RIGHT)),
			UP		= (1 << static_cast<uint32_t>(eAxis::UP)),
			DOWN	= (1 << static_cast<uint32_t>(eAxis::DOWN))
		};
		static inline const Vector3& Axis(eAxis type) { return axis[static_cast<uint32_t>(type)]; }
		static inline const Vector3 Axis(fAxis flags) { 
			Vector3 ret = { 0, 0, 0 };
			for (uint32_t i = 0; i < static_cast<uint32_t>(eAxis::MAX); ++i) {
				fAxis f = static_cast<fAxis>(1<<i);
				if (!(static_cast<uint32_t>(f) & static_cast<uint32_t>(flags))) continue;
				ret += axis[i];
			}
			return ret;
		}

		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//

		Vector3& operator = (DirectX::XMVECTOR& other) noexcept;
		Vector3& operator = (const float other) noexcept;

		Vector3 operator * (const float other) const noexcept;
		Vector3 operator * (const Vector3& other) const noexcept;
		Vector3& operator *= (const float other) noexcept;
		Vector3& operator *= (const Vector3& other) noexcept;

		Vector3 operator / (const float other) const noexcept;
		Vector3 operator / (const Vector3& other) const noexcept;
		Vector3& operator /= (const float other) noexcept;
		Vector3& operator /= (const Vector3& other) noexcept;

		Vector3 operator + (const Vector3& other) const noexcept;
		Vector3& operator += (const Vector3& other) noexcept;
		Vector3 operator - (const Vector3& other) const noexcept;
		Vector3& operator -= (const Vector3& other) noexcept;
		Vector3 operator - () const noexcept;

		//-----------------------------------------------------------------------------------------------------
		//
		// inline function
		//
		float	dot(const Vector3& v)	noexcept;
		float	angle(const Vector3& v) const noexcept;
		void	normalize()				noexcept;
		Vector3	cross(const Vector3& v) const noexcept;
		float	length()				const noexcept;
		Vector3	xy()					const noexcept;
		Vector3	xz()					const noexcept;
		Vector3	yz()					const noexcept;
		DirectX::XMFLOAT4 float4()		const noexcept;

		//-----------------------------------------------------------------------------------------------------
		//
		// static inline function
		//
		static Vector3	Normalize(const Vector3& v) noexcept;
		static Vector3	Lerp(const tnl::Vector3& s, const tnl::Vector3& e, float t) noexcept;
		static float	Dot(const tnl::Vector3& v1, const tnl::Vector3& v2) noexcept;
		static Vector3	Cross(const tnl::Vector3& v1, const tnl::Vector3& v2) noexcept;
		static Vector3	Rot2D(const tnl::Vector3 v, float sin, float cos) noexcept;
		static Vector3	Transform(const tnl::Vector3& v, const tnl::Matrix& m) noexcept;
		static Vector3	TransformNormal(const tnl::Vector3& v, const tnl::Matrix& m) noexcept;
		static Vector3	Reflection(const tnl::Vector3& in, const tnl::Vector3& normal) noexcept;

		//-----------------------------------------------------------------------------------------------------
		//
		// static function
		//
		static Vector3 AlongPlane(const tnl::Vector3& in, const tnl::Vector3& pn) noexcept ;
		static Vector3 TransformCoord(const Vector3& v, const tnl::Matrix& m) noexcept;
		static Vector3 TransformCoord(const Vector3 &v, const Quaternion &q) noexcept ;
		static Vector3 CreateScreenRay(const int screen_x, const int screen_y, const int screen_w, const int screen_h, const tnl::Matrix& view, const tnl::Matrix& proj) noexcept ;
		static Vector3 ConvertToScreen(const Vector3& v, const float screen_w, const float screen_h, const Matrix& view, const Matrix& proj) noexcept;
		static Vector3 Random( const float min_x, const float max_x, const float min_y, const float max_y, const float min_z, const float max_z ) noexcept;

	private:
		static const Vector3 axis[static_cast<uint32_t>(eAxis::MAX)];
	};

	constexpr Vector3::fAxis operator | (Vector3::fAxis left, Vector3::fAxis right) {
		return static_cast<Vector3::fAxis>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
	}
	constexpr bool operator & (Vector3::fAxis left, Vector3::fAxis right) {
		return static_cast<bool>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator = (DirectX::XMVECTOR& other) noexcept {
		DirectX::XMStoreFloat3(this, other);
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator = (const float other) noexcept {
		this->x = other;
		this->y = other;
		this->z = other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	//inline Vector3 Vector3::operator * (const Vector3& other) const noexcept {
	//	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(this);
	//	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&other);
	//	DirectX::XMVECTOR v = DirectX::XMVectorMultiply(l, r);
	//	DirectX::XMFLOAT3 f3;
	//	DirectX::XMStoreFloat3(&f3, v);
	//	return static_cast<Vector3>(f3);
	//}
	inline Vector3 Vector3::operator * (const Vector3& other) const noexcept {
		return { x * other.x, y * other.y, z * other.z };
	}
	//-----------------------------------------------------------------------------------------------------
	//inline Vector3 Vector3::operator * (const float other) const noexcept {
	//	DirectX::XMFLOAT3 t(other, other, other);
	//	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(this);
	//	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&t);
	//	DirectX::XMVECTOR v = DirectX::XMVectorMultiply(l, r);
	//	DirectX::XMFLOAT3 f3;
	//	DirectX::XMStoreFloat3(&f3, v);
	//	return static_cast<Vector3>(f3);
	//}
	inline Vector3 Vector3::operator * (const float other) const noexcept {
		return { x * other, y * other, z * other };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator *= (const float other) noexcept {
		*this = *this * other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator *= (const Vector3& other) noexcept {
		Vector3 v = other;
		*this = *this * v;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	//inline Vector3 Vector3::operator / (const float other) const noexcept {
	//	DirectX::XMFLOAT3 t(other, other, other);
	//	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(this);
	//	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&t);
	//	DirectX::XMVECTOR v = DirectX::XMVectorDivide(l, r);
	//	DirectX::XMFLOAT3 f3;
	//	DirectX::XMStoreFloat3(&f3, v);
	//	return static_cast<Vector3>(f3);
	//}
	inline Vector3 Vector3::operator / (const float other) const noexcept {
		return { x / other, y / other, z / other };
	}
	//-----------------------------------------------------------------------------------------------------
	//inline Vector3 Vector3::operator / (const Vector3& other) const noexcept {
	//	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(this);
	//	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&other);
	//	DirectX::XMVECTOR v = DirectX::XMVectorDivide(l, r);
	//	DirectX::XMFLOAT3 f3;
	//	DirectX::XMStoreFloat3(&f3, v);
	//	return static_cast<Vector3>(f3);
	//}
	inline Vector3 Vector3::operator / (const Vector3& other) const noexcept {
		return { x / other.x, y / other.y, z / other.z };
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator /= (const float other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator /= (const Vector3& other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::operator + (const Vector3& other) const noexcept {
		DirectX::XMFLOAT3 f3(this->x + other.x, this->y + other.y, this->z + other.z);
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator += (const Vector3& other) noexcept {
		*this = *this + other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::operator - (const Vector3& other) const noexcept {
		DirectX::XMFLOAT3 f3(this->x - other.x, this->y - other.y, this->z - other.z);
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3& Vector3::operator -= (const Vector3& other) noexcept {
		*this = *this - other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::operator-() const noexcept {
		Vector3 v = {-x, -y, -z};
		return v;
	}

	//-----------------------------------------------------------------------------------------------------
	inline float Vector3::dot(const Vector3& v) noexcept {
		return DirectX::XMVector3Dot(DirectX::XMLoadFloat3(this), DirectX::XMLoadFloat3(&v) ).m128_f32[0];
	}
	//-----------------------------------------------------------------------------------------------------
	inline float Vector3::angle(const Vector3& v) const noexcept {
		Vector3 t1 = *this;
		Vector3 t2 = v;
		t1.normalize();
		t2.normalize();
		return acosf(t1.dot(t2));
	}
	//-----------------------------------------------------------------------------------------------------
	inline void Vector3::normalize() noexcept {
		DirectX::XMStoreFloat3(this, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(this)));
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::cross(const Vector3& v) const noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(this), DirectX::XMLoadFloat3(&v)));
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	inline float Vector3::length() const noexcept {
		return DirectX::XMVector3Length(DirectX::XMLoadFloat3(this)).m128_f32[0];
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::xz() const noexcept { return { x, 0, z }; }
	inline Vector3 Vector3::xy() const noexcept { return { x, y, 0 }; }
	inline Vector3 Vector3::yz() const noexcept { return { 0, y, z }; }

	//-----------------------------------------------------------------------------------------------------
	inline DirectX::XMFLOAT4 Vector3::float4() const noexcept { return { x, y, z, 1.0f }; }

	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::Normalize(const Vector3& v) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v)));
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::Lerp(const tnl::Vector3& s, const tnl::Vector3& e, float t) noexcept {
		return s + (e - s) * t;
	}
	//-----------------------------------------------------------------------------------------------------
	inline float Vector3::Dot(const tnl::Vector3& v1, const tnl::Vector3& v2) noexcept {
		return DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2)).m128_f32[0];
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::Cross(const tnl::Vector3& v1, const tnl::Vector3& v2) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2)));
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::Rot2D(const tnl::Vector3 v, float sin, float cos) noexcept {
		return Vector3(v.x * cos - v.y * sin, v.x * sin + v.y * cos, 0.0);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector3 Vector3::Reflection( const tnl::Vector3& in, const tnl::Vector3& normal ) noexcept {
		return tnl::Vector3::Normalize(in - ( (normal * tnl::Vector3::Dot(in, normal) * 2.0f ) ));
	}



}
