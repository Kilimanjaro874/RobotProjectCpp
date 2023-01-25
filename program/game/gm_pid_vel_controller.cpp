#include "gm_pid_vel_controller.h"

tol::PIDVelController::PIDVelController(float horzontal_max, float vertical_max, float kp, float ki, float kd) {
	horizontal_max_ = horzontal_max;
	vertical_max_ = vertical_max;
	kp_ = kp;
	ki_ = ki;
	kd_ = kd;
}

tnl::Vector3 tol::PIDVelController::update(float delta_time, const tnl::Vector3& vel_dir, const tnl::Vector3& vel_current)
{
	// --- update vel_ref --- //
	vel_ref_ += vel_dir;
	vel_ref_.normalize();
	vel_ref_ = { vel_dir.x * horizontal_max_, vel_dir.y * vertical_max_, vel_dir.z * horizontal_max_ };
	vel_error_ = vel_ref_ - vel_current;
	// --- PID controll --- //
	vel_error_integral_ += (vel_error_ + vel_error_pre_) / 2 * delta_time;
	tnl::Vector3 force = vel_error_ * kp_ +
		vel_error_integral_ * ki_ +
		(vel_error_ - vel_error_pre_) / delta_time;
	// -- store info -- //
	vel_error_pre_ = vel_error_;
	return force;		// add force (N)
}

