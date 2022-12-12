#include "gm_camera.h"

void GmCamera::update() 
{
	tnl::Quaternion xq = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, _free_look_angle_xy.x);
	tnl::Quaternion yq = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, _free_look_angle_xy.y);
	//tnl::Quaternion zq = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, _free_look_angle_xy.z);

	tnl::Quaternion wq = xq * yq;
	pos_ = target_ + tnl::Vector3::TransformCoord({ 0, 0, -_target_distance }, wq);
	_focus_dir_tmp = _focus_dir.TransformCoord(_focus_dir, wq);
	_aim_pos = pos_ + _focus_dir_tmp * _aim_distance;
	DrawStringEx(50, 120, -1, "f = %f, %f, %f", _aim_pos.x, _aim_pos.y, _aim_pos.z);
	dxe::Camera::update();
}
