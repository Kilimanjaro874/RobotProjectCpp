#include "gm_camera_director.h"

void tol::TPSCamDirector::update(float delta_time, GmCamera* camera,
	std::shared_ptr<Object> cam_set_pos_ref, const tnl::Vector3 cam_set_offset) {
	// ---- Camera rotate ---- //
	GetMousePoint(&mouse_inputs_st_.x_delta_move_, &mouse_inputs_st_.y_delta_move_);
	mouse_inputs_st_.x_delta_move_ = mouse_inputs_st_.x_delta_move_ - DXE_WINDOW_WIDTH / 2;
	mouse_inputs_st_.y_delta_move_ = mouse_inputs_st_.y_delta_move_ - DXE_WINDOW_HEIGHT / 2;
	camera->free_look_angle_xy_ += tnl::Vector3{
		(float)mouse_inputs_st_.y_delta_move_,
		(float)mouse_inputs_st_.x_delta_move_,
		0 } *delta_time * cam_rot_coeff;
	// ---- Camera forcus_dir ---- //
	forcus_point_ = camera->pos_ + camera->focus_dir_tmp_ * focal_length_;
	camera->target_ = forcus_point_;	// update camera class->target_;
	// ---- Camera position ---- //
	auto cod = cam_set_pos_ref->getCoordinate();
	camera->pos_ = cod->getPos() + cam_set_offset;
}