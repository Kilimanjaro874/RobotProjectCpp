#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"
#include "gm_actor.h"

tol::PhysicsHandler::PhysicsHandler(float mass) {
	mass_ = mass;
}

void tol::PhysicsHandler::update(float delta_time,  std::shared_ptr<Actor> actor, const tnl::Vector3& vel_dir)
{
	
	auto cod = actor->getCoordinate();
	tnl::Vector3 vel = tnl::Vector3::TransformCoord(vel_dir, cod->getRot());
	tnl::Vector3 pow = pid_vel_cont_->update(delta_time, vel, getVelocity());
	acc_ = pow / mass_;
	velocity_ += acc_ * delta_time;
}


