#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Object;
	class Kinematics {
	public:
		Kinematics() {}
		virtual ~Kinematics() {}

	//// ---- Member variables ---- ////
	private:
		struct dk_data_st {		// store coordinate difference info. (this - parent)
			tnl::Vector3 dir_c_p_;
			float len_c_p_;
		};
		dk_data_st dk_data_st_ = { {0, 0, 0}, 1.0 };
		dk_data_st dk_data_upd_st_ = { {0, 0, 0}, 1.0 };
		bool is_dk_init = false;
		tnl::Quaternion rot_one_flame_;		// rotation of 1 flame.
	//// ---- Member functions ---- ////
		enum class ik_type {
			// object - target //
			pos_to_pos,
			dirx_as_dirx, dirx_as_diry, dirx_as_dirz,
			diry_as_diry, diry_as_dirx, diry_as_dirz,
			dirz_as_dirz, dirz_as_dirx, dirz_as_diry,
			dirx_look_pos,
			diry_look_pos,
			dirz_look_pos,
			as_it_was,
		};
		enum class ik_const {
			// object - target //
			rot_as_rot
		};

	public:
		virtual void init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child);
		virtual void update(float delta_time, std::shared_ptr<Object> obj);

	private:
		void initDKSetting(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child);
		void directKinematics(float delta_time, std::shared_ptr<Object> obj);
	public:
		// ---- setter ---- //
		void setDkData(const tnl::Vector3& dir, float length) {
			dk_data_st_.dir_c_p_ = dir;
			dk_data_st_.len_c_p_ = length;
		}
		void setRotDkData(const tnl::Quaternion rot) {
			dk_data_upd_st_.dir_c_p_ = tnl::Vector3::TransformCoord(dk_data_st_.dir_c_p_, rot);
		}
		void setRotOneFlame(tnl::Quaternion rot) { rot_one_flame_ = rot; }
		void setAddRotOneFlame(tnl::Quaternion rot) { rot_one_flame_ *= rot; }
		// ---- getter ---- //
		tnl::Quaternion getRotOneFlame(){ return rot_one_flame_; }

	};
}