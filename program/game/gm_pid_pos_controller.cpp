#include "gm_pid_pos_controller.h"

tol::PIDPosController::PIDPosController(float kp, float ki, float kd)
{
	kp_ = kp;
	ki_ = ki;
	kd_ = kd;
}

tnl::Vector3 tol::PIDPosController::update(float delta_time, const tnl::Vector3& pos_ref, const tnl::Vector3& pos_current)
{
	tnl::Vector3 pos_target;
	if (is_automatic_location_update_) {
		pos_target = locations_[move_point_num_];
		// -- reached check -- //
		tnl::Vector3 diff = pos_target - pos_current;
		float diff_length = diff.length();
		if (diff_length < position_error_tolerance_) {
			move_point_num_++;
			if (move_point_num_ >= locations_.size()) {
				move_point_num_ = 0;
			}
		}
	}
	else {
		pos_target = pos_ref;
	}
	// --- update : Force(N) to converge to the target position --- //
	pos_error_ = pos_target - pos_current;
	// --- PID Controll --- //
	pos_error_integral_ += (pos_error_ + pos_error_pre_) / 2 * delta_time;
	tnl::Vector3 force = pos_error_ * kp_ +
		pos_error_integral_ * ki_ +
		(pos_error_ - pos_error_pre_) / delta_time;
	// -- store info -- //
	pos_error_pre_ = pos_error_;
	return force;		// add force(N)
	
}

void tol::PIDPosController::setAutomaticLocationUpdate(std::vector<tnl::Vector3> locations, float error_torerance) {
	is_automatic_location_update_ = true;
	locations_ = locations;
	position_error_tolerance_ = error_torerance;
}