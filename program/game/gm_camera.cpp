#include "gm_camera.h"

void GmCamera::update() 
{
	// ----- カメラ位置＆姿勢更新 ----- //
	// ---- 姿勢 ---- //
	tnl::Quaternion xq = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, free_look_angle_xy_.x);
	tnl::Quaternion yq = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, free_look_angle_xy_.y);
	tnl::Quaternion wq = xq * yq;
	// ---- 位置、方向ベクトル ---- //
	//pos_ = target_ + tnl::Vector3::TransformCoord({ 0, 0, -target_distance_ }, wq);
	focus_dir_tmp_ = focus_dir_.TransformCoord(focus_dir_, wq);
	//_aim_pos = pos_ + _focus_dir_tmp * _aim_distance;
	// DrawStringEx(50, 120, -1, "f = %f, %f, %f", _aim_pos.x, _aim_pos.y, _aim_pos.z);
	dxe::Camera::update();
}