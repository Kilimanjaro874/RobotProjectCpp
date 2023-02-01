#include "gm_pid_rot_controller.h"
#include <cmath>

tol::PIDRotController::PIDRotController(float rotation_speed_max, float tolerance_deg, float kp, float ki, float kd)
{
	rotation_speed_max_ = rotation_speed_max;
	float tolerance_deg_ = tolerance_deg;
	kp_ = kp;
	ki_ = ki;
	kd_ = kd;
}

float tol::PIDRotController::update(float delta_time, const tnl::Vector3& look_ref_pos, const tnl::Vector3& cureent_dir)
{
	// --- get rotate direction --- //
	tnl::Vector3 ref_dir = { look_ref_pos.x, 0, look_ref_pos.z };
	ref_dir.normalize();
	tnl::Vector3 axis = { 0, 1, 0 };
	int dir = axis.dot(ref_dir.cross(cureent_dir)) >= 0 ? 1 : -1;	 // get dir;
	// --- calc pid control --- //
	angle_error_ = acos(tnl::Vector3::Dot(ref_dir, cureent_dir));
	if (1.0 - cos(tnl::ToRadian(90 - tolerance_deg_)) < angle_error_) { angle_error_ = 0; }
	angle_error_integral_ += (angle_error_ + angle_error_pre_) / 2 * delta_time;
	float torque = angle_error_ * kp_ +
		angle_error_integral_ * ki_ +
		(angle_error_ - angle_error_pre_) / delta_time * kd_;
	// -- store info -- //
	angle_error_pre_ = angle_error_;
	return dir * torque;
}
