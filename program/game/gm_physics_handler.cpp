#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"
#include "gm_actor.h"

tol::PhysicsHandler::PhysicsHandler(float mass, float inertia_y, float velocity_limit, float ang_vel_limit) {
	mass_ = mass;
	inertia_y_ = inertia_y;
	velocity_max_ = velocity_limit;
	ang_vel_limit_ = ang_vel_limit;
}

void tol::PhysicsHandler::update(float delta_time,  std::shared_ptr<Object> actor, const tnl::Vector3& force, float torque)
{
	// transform
	if (force.length() != 0) {
		acc_ = force / mass_;
		if (velocity_.length() < velocity_max_) {
			velocity_ += acc_ * delta_time;
		}
	}
	// rotation
	if (torque != 0) {
		ang_acc_ = torque / inertia_y_;
		if (std::fabsf(ang_vel_) < ang_vel_limit_) {
			ang_vel_ += ang_acc_ * delta_time;
		}
	}
}