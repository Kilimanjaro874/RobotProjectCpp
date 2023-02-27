#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_physics_handler.h"

namespace tol {
	class PhysicsHandler;

	class PIDRotController {
	public:
		PIDRotController(float tolerance_deg, float kp, float ki, float kd);
		virtual ~PIDRotController() {}

	private:
		//// ---- Member variables ---- ////
		// --- define --- //
		float tolerance_deg_;			// allowable angular 
		float kp_;
		float ki_;
		float kd_;
		// --- calc params --- //
		float angle_error_ = 0;				// error(ref - current param.)
		float angle_error_pre_ = 0;			// error befor 1 frame.
		float angle_error_integral_ = 0;	// sum of error.
		//// ---- Member variables ---- ////
	public:
		virtual float update(float delta_time, const tnl::Vector3& look_ref_pos, const tnl::Vector3& cureent_dir);
	};
}