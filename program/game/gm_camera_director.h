#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext_camera.h"
#include "gm_camera.h"
#include "gm_object.h"

namespace tol {

	class TPSCamDirector {
	public:
		TPSCamDirector(){}
		virtual ~TPSCamDirector() {}
	
	protected:
		//// ----- Member variables ------ ////
		// --- define --- //
		tnl::Vector3 cam_target_offset_;
		tnl::Vector3 cam_pos_offset_;

		//// ----- Member functions ----- ////
	public:
		virtual void update(float delta_time, GmCamera camera, std::shared_ptr<Object> target, );
	};
}