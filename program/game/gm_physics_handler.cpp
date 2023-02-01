#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"
#include "gm_actor.h"

tol::PhysicsHandler::PhysicsHandler(float mass, float inertia_y) {
	mass_ = mass;
	inertia_y_ = inertia_y;
}

void tol::PhysicsHandler::update(float delta_time,  std::shared_ptr<Object> actor, const tnl::Vector3& force, float torque)
{
	// transform
	if (force.length() != 0) {
		acc_ = force / mass_;
		velocity_ += acc_ * delta_time;
	}
	// rotation
	if (torque != 0) {
		ang_acc_ = torque / inertia_y_;
		ang_vel_ += ang_acc_ * delta_time;
	}
}


