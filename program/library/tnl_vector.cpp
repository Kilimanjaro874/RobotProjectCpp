#include "tnl_math.h"
#include "tnl_vector.h"
#include "tnl_quaternion.h"
#include "tnl_matrix.h"

namespace tnl {

	using namespace DirectX;

	const Vector3 Vector3::front	= { 0,  0,  1 };
	const Vector3 Vector3::back		= { 0,  0, -1 };
	const Vector3 Vector3::left		= {-1,  0,  0 };
	const Vector3 Vector3::right	= { 1,  0,  0 };
	const Vector3 Vector3::up		= { 0,  1,  0 };
	const Vector3 Vector3::down		= { 0, -1,  0 };

	const Vector3 Vector3::axis[static_cast<uint32_t>(eAxis::MAX)] = {
		Vector3::front,
		Vector3::back,
		Vector3::left,
		Vector3::right,
		Vector3::up,
		Vector3::down
	};

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::AlongPlane(const tnl::Vector3& in, const tnl::Vector3& pn) noexcept {
		tnl::Vector3 c = tnl::Vector3::Cross(in, pn);
		tnl::Vector3 along = tnl::Vector3::Cross(pn, c);
		return Normalize(along);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::TransformCoord(const Vector3& v, const Quaternion& q) noexcept {
		XMVECTOR xv = XMVector3Rotate( XMLoadFloat3(&v), XMLoadFloat4(&q));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, xv); 
		return static_cast<Vector3>(f3);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::TransformCoord(const Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::Transform(const tnl::Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}
	Vector3 Vector3::TransformNormal(const tnl::Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}


	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::CreateScreenRay(const int screen_x, const int screen_y, const int screen_w, const int screen_h, const tnl::Matrix& view, const tnl::Matrix& proj) noexcept {

		Matrix mv, mp;
		mv = view;
		mp = proj;

		mv._41 = 0.0f;
		mv._42 = 0.0f;
		mv._43 = 0.0f;

		// ビューポート行列を作成
		Matrix view_mat;

		view_mat._11 = (float)(screen_w >> 1);
		view_mat._22 = -1.0f * (float)(screen_h >> 1);
		view_mat._33 = 1.0f;
		view_mat._41 = (float)(screen_w >> 1);
		view_mat._42 = (float)(screen_h >> 1);
		view_mat._43 = 0.0f;

		// スクリーン位置をワールドへ
		Vector3 v;
		v.x = (float)screen_x;
		v.y = (float)screen_y;
		v.z = 0.0f;

		Matrix inv, inv_proj, inv_view;
		inv = Matrix::Inverse(view_mat);
		inv_proj = Matrix::Inverse(mp);
		inv_view = Matrix::Inverse(mv);
		//inv = inv_view * inv_proj * inv;
		inv = inv * inv_proj * inv_view;

		return Vector3::TransformCoord(v, inv);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::ConvertToScreen(const Vector3& v, const float screen_w, const float screen_h, const Matrix& view, const Matrix& proj) noexcept {

		Vector3 rv(0, 0, 0);
		float w = screen_h * 0.5f;
		float h = screen_w * 0.5f;
		Matrix wkMtx, tm;

		tm = Matrix::Translation(v);
		//wkMtx = MatrixMultiply(_proj, _view);
		//wkMtx = MatrixMultiply(wkMtx, T);
		wkMtx = view * proj;
		wkMtx = tm * wkMtx ;

		wkMtx._14 /= wkMtx._44;
		wkMtx._24 /= wkMtx._44;
		rv.x = (h + (-h * wkMtx._24));
		rv.y = (w - (w * wkMtx._14));
		return v;
	}

	Vector3 Vector3::Random(
		const float min_x, const float max_x,
		const float min_y, const float max_y,
		const float min_z, const float max_z) noexcept{
		Vector3 v = {
			tnl::GetRandomDistributionFloat(min_x, max_x),
			tnl::GetRandomDistributionFloat(min_y, max_y),
			tnl::GetRandomDistributionFloat(min_z, max_z)
		};
		return v;
	}


}

