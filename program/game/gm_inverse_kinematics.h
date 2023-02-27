#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Object;
	class Kinematics;

	class InvKinematics {
	public:
		InvKinematics() {}
		virtual ~InvKinematics() {}

		//// ---- Member variables ---- ////
	public:
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
	private:
		struct ik_data_st {
			int ik_id_;
			std::string ik_name_;
			bool is_do_ik_ = true;
			std::weak_ptr<Object> target_;
			std::weak_ptr<Object> object_;
			ik_type ik_type_;
			float kp_;
			bool is_rot_[static_cast<int>(Coordinate::coordinate::end)] = { true, true, true };
		};
		std::vector<ik_data_st> ik_data_st_;
		bool is_ik_init_ = false;
		//// ---- Member functions ---- ////
	public:
		virtual void init(const std::shared_ptr<Object> target, const std::shared_ptr<Object> object,
			int ik_id, std::string ik_name,
			ik_type type, float kp, bool is_x_rot, bool is_y_rot, bool is_z_rot);
		virtual tnl::Quaternion update(float delta_time, std::shared_ptr<Object> obj);

	private:
		tnl::Quaternion inverseKinematics(float delta_time, std::shared_ptr<Object> obj);
	};
}