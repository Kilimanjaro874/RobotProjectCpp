#include "gm_tps_camera_director.h"

tol::TPSCameraDirector::TPSCameraDirector(dxe::Camera* camera, const tnl::Vector3 cam_pos, std::shared_ptr<Object> tar_obj)
{
	camera = new dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	camera->pos_ = cam_pos;
	auto tar_cod = tar_obj->getCoordinate();
	camera->target_ = tar_cod->getPos();
	// --- define init --- //
	// get direction
	forcus_dir_ = tar_cod->getPos() - camera->pos_;
	forcus_dir_.normalize();
	forcus_dir_upd_ = forcus_dir_;
	forcus_point_ = forcus_dir_ * aim_distance_;
	rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	// get rot axis 
	tnl::Vector3 tmp_dir_y = { 0, 1, 0 };
	tnl::Vector3 tmp_dir_x = forcus_dir_.cross(tmp_dir_y);
	rot_axis_x_ = tmp_dir_x;
	rot_axis_x_.normalize();
	rot_axis_x_upd_ = rot_axis_x_;
	rot_axis_y_ = rot_axis_x_.cross(forcus_dir_);
	rot_axis_y_.normalize();
	rot_axis_y_upd_ = rot_axis_y_;
}

void tol::TPSCameraDirector::update(float delta_time, dxe::Camera* camera, std::shared_ptr<Object> tar_obj)
{
	// ---- TPS camera rotate update ---- //
	GetMousePoint(&mouse_inputs_st_.x_delta_move_, &mouse_inputs_st_.y_delta_move_);
	mouse_inputs_st_.x_delta_move_ = (mouse_inputs_st_.x_delta_move_ - DXE_WINDOW_WIDTH / 2);
	mouse_inputs_st_.y_delta_move_ = (mouse_inputs_st_.y_delta_move_ - DXE_WINDOW_HEIGHT / 2);
	float angle_move_x = -(float)mouse_inputs_st_.y_delta_move_ * delta_time * cam_rot_coeff;
	float angle_move_y = (float)mouse_inputs_st_.x_delta_move_ * delta_time * cam_rot_coeff;
	tnl::Quaternion xq = tnl::Quaternion::RotationAxis(rot_axis_x_upd_, angle_move_x);
	tnl::Quaternion yq = tnl::Quaternion::RotationAxis(rot_axis_y_upd_, angle_move_y);
	rot_ *= xq * yq;
	tnl::Vector3 angle = rot_.getEuler();
	DrawStringEx(50, 200, -1, "angle = %f, %f, %f", angle.x, angle.y, angle.z);
	// --- update defines --- //
	forcus_dir_upd_ = forcus_dir_.TransformCoord(forcus_dir_, rot_);
	forcus_point_ = forcus_dir_upd_ * aim_distance_;
	rot_axis_x_upd_ = rot_axis_x_.TransformCoord(rot_axis_x_, rot_);
	rot_axis_y_upd_ = rot_axis_y_.TransformCoord(rot_axis_y_, rot_);
	// --- update camera state --- //
	auto tar_cod = tar_obj->getCoordinate();
	camera->pos_ = tar_cod->getPos() - forcus_dir_upd_ * tar_cam_distance_;
	camera->target_ = tar_cod->getPos();
}
