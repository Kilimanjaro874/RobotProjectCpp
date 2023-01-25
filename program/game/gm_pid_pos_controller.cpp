#include "gm_pid_pos_controller.h"

tol::PIDPosController::PIDPosController(float kp, float ki, float kd)
{
	kp_ = kp;
	ki_ = ki;
	kd_ = kd;
}

tnl::Vector3 tol::PIDPosController::update(float delta_time, const tnl::Vector3& pos_ref, const tnl::Vector3& pos_current)
{
	// --- update : Force(N) to converge to the target position --- //
	pos_error_ = pos_ref - pos_current;
	// --- PID Controll --- //
	pos_error_integral_ += (pos_error_ + pos_error_pre_) / 2 * delta_time;
	tnl::Vector3 force = pos_error_ * kp_ +
		pos_error_integral_ * ki_ +
		(pos_error_ - pos_error_pre_) / delta_time;
	// -- store info -- //
	pos_error_pre_ = pos_error_;
	return force;		// add force(N)
}
