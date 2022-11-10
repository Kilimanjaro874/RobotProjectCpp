#include "gm_robot_controller.h"

void RobotCont::update(float delta_time) {
	// ----- �v���C���[����̉e�������{�b�g�N���X�ɗ^���� ----- //
	
	// ---- ������ ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- ���͎�t ---- //
	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		d_move = _robot->_dir_z_tmp * delta_time * _move_speed;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		d_move = _robot->_dir_z_tmp * delta_time * -1 * _move_speed;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, _rot_speed * delta_time);
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, -1 * _rot_speed * delta_time);
	} 
	// ---- DK�p�Ƀp�����[�^���i�[ ---- //
	_dk_st_next._dir = _robot->_pos + d_move;
	_dk_st_next._rot_sum = rot_move;
}
