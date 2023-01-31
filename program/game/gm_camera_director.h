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
		tnl::Vector3 
		tnl::Vector3 cam_target_offset_;
		tnl::Vector3 cam_pos_offset_;

		//// ----- Member functions ----- ////
	public:
		virtual void update(float delta_time, GmCamera camera, 
			std::shared_ptr<Object> target, const tnl::Vector3& tar_offset_pos_,
			std::shared_ptr<Object> object, const tnl::Vector3& obj_offset_pos_);
	};
}