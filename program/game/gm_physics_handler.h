#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Actor;
	class PIDVelController;

	class PhysicsHandler {
	public:
		PhysicsHandler(float mass, float inertia_y);
		virtual ~PhysicsHandler() {}
	protected:
		//// ---- Member variables ---- ////
		// --- define --- //
		float mass_;			// kg
		float inertia_y_;		// moment of inertia about the y-axis kgm2
		// --- calc params --- //
		// -- translate -- //
		tnl::Vector3 acc_ = { 0, 0, 0 };		// m/s2
		tnl::Vector3 velocity_ = { 0, 0, 0 };	// m/s
		// -- rotate -- //
		float ang_acc_ = 0;				// rad/s2
		float ang_vel_ = 0;				// rad/s
		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time, std::shared_ptr<Object> actor, const tnl::Vector3& force_, float torque = 0);
		// ---- setter ---- //
		// ---- getter ---- //
		tnl::Vector3 getVelocity() { return velocity_; }
		float getRotVelocity() { return ang_vel_; }

	};	
}