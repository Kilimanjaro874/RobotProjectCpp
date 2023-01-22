#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"

tol::PhysicsHandler::PhysicsHandler(float mass) {
	mass_ = mass;
}

void tol::PhysicsHandler::update(float delta_time, const tnl::Vector3& vel_dir)
{
	tnl::Vector3 pow = pid_vel_cont_->update(delta_time, vel_dir, getVelocity());

}


