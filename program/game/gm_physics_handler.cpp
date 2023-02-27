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
	// effect of gravity (****  temporary expedient ***** )
	if (is_gravity_enebled_) {
		// constrain
		//velocity_ += tnl::Vector3{ 0, gravitational_acc_, 0 } *delta_time;
		auto actor_pos = actor->getCoordinate()->getPos();
		if (actor_pos.y < floor_y_) {
			actor->getCoordinate()->setPos({ actor_pos.x, 0, actor_pos.z });
			
		}
		else {
			float actor_y = actor_pos.y;
			actor_y += gravitational_acc_ * delta_time;
			actor->getCoordinate()->setPos({ actor_pos.x, actor_y, actor_pos.z });
		}
		
	}

	// transform
	DrawStringEx(300, 130, -1, "acc_-> x:%5.2f, y:%5.2f, z:%5.2f", acc_.x, acc_.y, acc_.z);
	DrawStringEx(700, 130, -1, "force_-> x:%5.2f, y:%5.2f, z:%5.2f", force.x, force.y, force.z);
	if (force.length() != 0) {
		acc_ = force / mass_;
		velocity_ += acc_ * delta_time;
		if (velocity_.length() > velocity_max_) {
			velocity_.normalize();
			velocity_ = velocity_ * velocity_max_;
		}
	}
	
	auto aPos = actor->getCoordinate()->getPos();
	DrawStringEx(300, 160, -1, "vel_-> x:%5.2f, y:%5.2f, z:%5.2f", velocity_.x, velocity_.y, velocity_.z);
	DrawStringEx(300, 190, -1, "pos_-> x:%5.2f, y:%5.2f, z:%5.2f", aPos.x, aPos.y, aPos.z);
	// rotation
	if (torque != 0) {
		ang_acc_ = torque / inertia_y_;
		ang_vel_ += ang_acc_ * delta_time;
	}
	
}