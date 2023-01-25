#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"
#include "gm_actor.h"

tol::PhysicsHandler::PhysicsHandler(float mass) {
	mass_ = mass;
}

void tol::PhysicsHandler::update(float delta_time,  std::shared_ptr<Object> actor, const tnl::Vector3& force)
{
	acc_ = force / mass_;
	velocity_ += acc_ * delta_time;
}


