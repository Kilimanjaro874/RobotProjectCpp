#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext_camera.h"
#include "gm_camera.h"
#include "gm_object.h"

namespace tol {

	class TPSCamDirector {
	public:
		TPSCamDirector(){}
		virtual ~TPSCamDirector() {
		}
		
	protected:
		//// ----- Member variables ------ ////
		// --- define --- //
		tnl::Vector3 forcus_point_ = { 0, 0, 1 };	// camera look point.(ref : world coordinate)
		float focal_length_ = 500.0f;				// camera focus length. (m)
		float cam_tar_lentgh = 20.0f;				// camera to camera_target length;	 
		struct mouse_inputs {
			int x_delta_move_;		// mouse move x : from Center of Window (while 1flame)
			int y_delta_move_;		// mouse move y : from Center of Window (while 1flame)
		} mouse_inputs_st_ = { DXE_WINDOW_WIDTH/2, DXE_WINDOW_HEIGHT/2 };
		float cam_rot_coeff = 0.2f;	// Camera rotation sensitivity by mouse movement.

		//// ----- Member functions ----- ////
	public:
		virtual void update(float delta_time, GmCamera* camera,
			std::shared_ptr<Object> cam_set_pos_ref, const tnl::Vector3 cam_set_offset);
		// ----- getter ----- //
		// ----- setter ----- //
	};
}