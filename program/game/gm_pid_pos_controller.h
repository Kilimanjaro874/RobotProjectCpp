#pragma once

#include<memory>
#include"../dxlib_ext/dxlib_ext.h"
#include "gm_physics_handler.h"

namespace tol {
	class PhysicsHandler;

	class PIDPosController {
	public:
		PIDPosController(float kp, float ki, float kd);
		virtual ~PIDPosController() {}

	private:
		//// ---- Member variables ---- ////
		// --- define --- //
		float kp_;
		float ki_;
		float kd_;
		// --- calc params --- //
		tnl::Vector3 pos_error_;			// error(ref - current param.)
		tnl::Vector3 pos_error_pre_;		// error befor 1 frame.
		tnl::Vector3 pos_error_integral_;	// sum of error.
		//// ---- Memver functions ---- ////
	public:
		virtual tnl::Vector3 update(float delta_time, const tnl::Vector3& pos_ref, const tnl::Vector3& pos_current);
	};
}