#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"

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

		
		// --- for PID Controll --- //
		std::shared_ptr<PIDVelController> pid_vel_cont_ = nullptr;

		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time, const tnl::Vector3& vel_dir);
		// ---- setter ---- //
		void setPIDController(std::shared_ptr<PIDVelController> pid_vel_cont) { pid_vel_cont_ = pid_vel_cont; }
		// ---- getter ---- //
		tnl::Vector3 getVelocity() { return velocity_; }

	};	
}