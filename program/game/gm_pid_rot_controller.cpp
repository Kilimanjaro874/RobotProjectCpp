#include "gm_pid_rot_controller.h"
#include <cmath>

tol::PIDRotController::PIDRotController(float tolerance_deg, float kp, float ki, float kd)
{
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
	int dir = axis.dot(ref_dir.cross(cureent_dir)) >= 0 ? -1 : 1;	 // get dir;
	// --- calc pid control --- //
	angle_error_ = std::acosf(std::clamp(tnl::Vector3::Dot(ref_dir, cureent_dir), (float)-1, (float)1));
	if (!isfinite(angle_error_)) { angle_error_ = 0; }
	if (1.0 - cos(tnl::ToRadian(90 - tolerance_deg_)) < angle_error_) {
		angle_error_ = 0; 
		dir = 0;
	}
	angle_error_integral_ += (angle_error_ + angle_error_pre_) / 2 * delta_time;
	float torque = angle_error_ * kp_ +
		angle_error_integral_ * ki_ +
		(angle_error_ - angle_error_pre_) / delta_time * kd_;
	// -- store info -- //
	angle_error_pre_ = angle_error_;
	
	//DrawStringEx(50, 30, -1, "look_ref_pos = %f, %f, %f", look_ref_pos.x, look_ref_pos.y, look_ref_pos.z);
	//DrawStringEx(50, 50, -1, "angle_error = %f", tnl::ToDegree(angle_error_));
	//DrawStringEx(50, 70, -1, "dir = %d", dir);
	//DrawStringEx(50, 90, -1, "ref_dir x = %5.2f, %5.2f", ref_dir.x, ref_dir.z);
	//DrawStringEx(50, 110, -1, "look_ref x = %5.2f, %5.2f", cureent_dir.x, cureent_dir.z);
	//DrawStringEx(50, 130, -1, "torque = %f", torque * dir);
	return dir * torque;
}