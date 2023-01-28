#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext_camera.h"
#include "gm_object.h"
#include "gm_actor.h"

namespace tol {

	class TPSCamera : public Object {
	public:
		TPSCamera(int id, std::string name) : Object(id, name) {
			screen_w_ = DXE_WINDOW_WIDTH;
			screen_h_ = DXE_WINDOW_HEIGHT;
			aspect_ = (float)screen_w_ / (float)screen_h_;
			Object::init();
		}
		virtual ~TPSCamera() {}
	
	protected:
		//// ----- Member variables ------ ////
		// ---- camera variables ---- //
		// --- define --- //
		tnl::Vector3 up_ = tnl::Vector3(0, 1, 0);	// Defines "above"the camera
		int screen_w_ = 0;
		int screen_h_ = 0;
		float angle_ = tnl::ToRadian(60.0f);	// angle of view. (deg)
		float aspect_ = 1.0f;					// Aspect ratio.
		// Close distance of the ranged to be photographed by the camera.
		float near_ = 1.0f;						
		// Maximum distance of the ranged to be photographed by the camera.
		float far_ = 5000.0f;
		
		tnl::Matrix view_;
		tnl::Matrix prj_;
		//// ----- Member functions ----- ////
		virtual void camUpdate(std::shared_ptr<tol::Object> obj, const tnl::Vector3& offset_pos);
	public:

	};
}