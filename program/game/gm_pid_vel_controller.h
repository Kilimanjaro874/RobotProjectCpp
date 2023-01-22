#pragma once

#include <memory>
#include "../dxlib_ext/dxlib_ext.h"

namespace tol {
	class PhysicsHandler;

	class PIDVelController {
	public:
		PIDVelController(float horzontal_max, float vertical_max, float kp, float ki, float kd);
		virtual ~PIDVelController() {}

	private:
		//// ---- Member variables ---- ////
		// --- define --- //
		float horizontal_max_;
		float vertical_max_;
		float kp_;
		float ki_;
		float kd_;
		// --- calc params. --- //
		tnl::Vector3 vel_ref_;				// set ideal reference data.
		tnl::Vector3 vel_error_;			// error(ref - current param.)
		tnl::Vector3 vel_error_pre_;		// error befor 1 frame.
		tnl::Vector3 vel_error_integral_;	// sum of error.

		//// ---- Member functions ---- ////
	public:
		virtual tnl::Vector3 update(float delta_time, const tnl::Vector3& vel_dir, const tnl::Vector3& vel_current);
	};
}