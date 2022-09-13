#include "tnl_matrix.h"
#include "tnl_quaternion.h"

namespace tnl {

	using namespace DirectX;

	void Matrix::rotationQuaternion(const tnl::Quaternion& q) noexcept {
		DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&q);
		DirectX::XMMATRIX xm = DirectX::XMMatrixRotationQuaternion(v);
		XMStoreFloat4x4(this, xm);
	}
	Matrix Matrix::RotationQuaternion(const tnl::Quaternion& q) noexcept {
		DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&q);
		DirectX::XMMATRIX xm = DirectX::XMMatrixRotationQuaternion(v);
		DirectX::XMFLOAT4X4 f4x4;
		XMStoreFloat4x4(&f4x4, xm);
		return static_cast<Matrix>(f4x4);
	}


}