#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace tol {
	class Coordinate {
	
	public:
		Coordinate() {};
		virtual ~Coordinate() {};

		//// ----- Member variables ---- ////
		enum class coordinate {	x, y, z, end };
	private:
		// ---- General ---- //
		tnl::Vector3 pos_;			// coordinate origine pos (��i)
		tnl::Quaternion rot_;		// coordinate rotation from ��0(World)
		std::vector<tnl::Vector3> c_vec_v_;			// orthonormal basis(initial value)
		std::vector<tnl::Vector3> c_vec_upd_v_;		// (update value)

		//// ------ Member functions ---- ////
	public:
		void init(
			tnl::Vector3 pos = { 0, 0, 0 },
			tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0),
			tnl::Vector3 dir_x = { 1, 0, 0 },
			tnl::Vector3 dir_y = { 0, 1, 0 },
			tnl::Vector3 dir_z = { 0, 0, 1 }
		);
		// ---- setter ---- //
		void setPos(const tnl::Vector3& pos) { pos_ = pos; }
		void setAddRot(const tnl::Quaternion& rot);
		// ---- getter ---- //
		tnl::Vector3 getPos() { return pos_; }
		tnl::Quaternion getRot() { return rot_; }
		tnl::Vector3 getDirX() { return c_vec_upd_v_[static_cast<int>(coordinate::x)]; }
		tnl::Vector3 getDirY() { return c_vec_upd_v_[static_cast<int>(coordinate::y)]; }
		tnl::Vector3 getDirZ() { return c_vec_upd_v_[static_cast<int>(coordinate::z)]; }
		
	};
}