#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h";
#include "gm_coordinate.h"

namespace tol {
	class Kinematics {
	public:
		Kinematics() {}
		~Kinematics() {}

		// ---- Direct Kinematics ---- //
		bool is_do_dk_ = true;
		/*enum class dk_attach_type {
			absolute, relative
		};*/
		struct dk_data_st {
			// store coordinate difference info. (parent - this(child)) 
			tnl::Vector3 dir_p_to_c_;
			float len_p_to_c_;
		};
		dk_data_st dk_data_st_ = { {0, 0, 0}, 1.0 };

		// ---- Inverse Kinematics ---- //
		bool is_do_ik_ = true;
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
		enum class ik_construction {
			// object - target //
			rot_as_rot,
		};
		struct ik_data_st {
			std::weak_ptr<Object> ik_target_;
			std::weak_ptr<Object> ik_object_;
			ik_type type_;
			float kp;
			bool is_can_rot_axis[static_cast<int>(Coordinate::coordinate::end)] = { true, true, true };
		};
		std::vector<ik_data_st> ik_data_st_;
		virtual void init(const Object& obj);
		virtual void update(float delta_time);
		void initDKSetting(const Object& obj);
	};
}