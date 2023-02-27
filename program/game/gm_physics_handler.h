#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Actor;
	class PIDVelController;

	class PhysicsHandler {
	public:
		PhysicsHandler(float mass, float inertia_y, float velocity_limit, float ang_vel_limit);
		virtual ~PhysicsHandler() {}
	protected:
		//// ---- Member variables ---- ////
		// --- define --- //
		float mass_;			// kg
		float inertia_y_;		// moment of inertia about the y-axis kgm2
		float gravitational_acc_ = -9.8;	// m/s2
		float floor_y_ = 0;
		bool is_gravity_enebled_ = false;
		// --- calc params --- //
		// -- translate -- //
		tnl::Vector3 acc_ = { 0, 0, 0 };		// m/s2
		tnl::Vector3 velocity_ = { 0, 0, 0 };	// m/s
		float velocity_max_ = 0.0;				// velocity limit (m/s)
		// -- rotate -- //
		float ang_acc_ = 0;				// rad/s2
		float ang_vel_ = 0;				// rad/s
		float ang_vel_limit_ = 0.0;		// angle vel limit(rad/s)
		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time, std::shared_ptr<Object> actor, const tnl::Vector3& force_, float torque = 0);
		// ---- setter ---- //
		void setIsAffectedByGravity(bool is_gravity_enabled) { is_gravity_enebled_ = is_gravity_enabled; }
		// ---- getter ---- //
		tnl::Vector3 getVelocity() { return velocity_; }
		float getRotVelocity() { return ang_vel_; }
		bool getIsAffectedByGravity() { return is_gravity_enebled_; }
	};	
}