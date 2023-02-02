#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {

	class TPSCameraDirector {
	public:
		TPSCameraDirector(dxe::Camera* camera, const tnl::Vector3 cam_pos, std::shared_ptr<Object> tar_obj);
		virtual ~TPSCameraDirector() {}
		//// ---- Member variables ---- ////
		// --- define --- //
		tnl::Vector3 forcus_dir_;			// direction of target_obj.(init);
		tnl::Vector3 forcus_dir_upd_;		// direction of target_obj.(update);
		tnl::Vector3 forcus_point_;			// direction & length of target_obj_.
		tnl::Vector3 rot_axis_x_;			// cam rotate axis(x). (init)
		tnl::Vector3 rot_axis_x_upd_;		// cam rotate axis(x). (update)
		tnl::Vector3 rot_axis_y_;			// cam rotate axis(y). (init)
		tnl::Vector3 rot_axis_y_upd_;		// cam rotate axis(y). (update)

		tnl::Quaternion rot_;				// rotation of this camera_.
		float aim_distance_ = 1000.0f;		// camera forcus length.(m)
		float tar_cam_distance_ = 20.0f;	// length of target_obj - this camera_.
		struct mouse_inputs {
			int x_delta_move_;		// mouse move x : from Center of Window (while 1flame)
			int y_delta_move_;		// mouse move y : from Center of Window (while 1flame)
		} mouse_inputs_st_ = { DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 };
		float cam_rot_coeff = 0.05f;	// Camera rotation sensitivity by mouse movement.
	protected:

	public:
		virtual void update(float delta_time, dxe::Camera* camera, std::shared_ptr<Object> tar_obj);
		// ---- setter ---- //
		// ---- getter ---- //
		tnl::Vector3 getForcusPos() { return forcus_point_; }
	};
}