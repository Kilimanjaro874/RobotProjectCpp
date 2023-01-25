#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Actor;
	class PIDVelController;

	class PhysicsHandler {
	public:
		PhysicsHandler(float mass);
		virtual ~PhysicsHandler() {}
	protected:
		//// ---- Member variables ---- ////
		// --- define --- //
		float mass_;			// kg
		// --- calc params --- //
		tnl::Vector3 acc_;
		tnl::Vector3 velocity_;
		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time, std::shared_ptr<Object> actor, const tnl::Vector3& force_);
		// ---- setter ---- //
		// ---- getter ---- //
		tnl::Vector3 getVelocity() { return velocity_; }

	};	
}