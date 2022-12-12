#include "gm_robot_controller.h"

void RobotCont::update(float delta_time, GmCamera* camera) {
	// ----- �v���C���[����̉e�������{�b�g�N���X�ɗ^���� ----- //
	// ---- ���͏��擾 ---- //
	input(delta_time);
	// ---- ���{�b�g�ړ��ʏ����� ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- TPS�J�������[�N ---- //
	camera->target_ = tnl::Vector3{ _robot->_pos.x, this->_camera_height, _robot->_pos.z };
	camera->target_distance_ = this->_camera_distance;
	DrawStringEx(50, 200, -1, "%2.5f, %2.5f, %2.5f", _robot->_pos.x, _robot->_pos.y, _robot->_pos.z);
	camera->free_look_angle_xy_ += tnl::Vector3{
		(float)_mouse_input_st.y_delta_move, 
		(float)_mouse_input_st.x_delta_move, 
		0
	} * delta_time * _camera_rot_coefficient;
	// ---- ���{�b�g�����ړ� ---- //
	d_move += _robot->_dir_z_tmp * delta_time * _move.y * _move_speed;
	d_move += _robot->_dir_x_tmp * delta_time * _move.x * _move_speed;
	// ---- ���{�b�g��]����(�J�����̌����Ɠ���) ---- //
	tnl::Vector3 cam_dir = camera->front();
	int angle_dir = getAngleDir(cam_dir, _robot->_dir_z_tmp);		// ���{�b�g�̉�]��������(0 or 1 or -1)
	rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, angle_dir * _rot_speed * delta_time);
	

	// ---- ���{�b�g�̍��W�ϊ��̂��߂̃p�����[�^(�����ړ��A��]�ړ�)���i�[ ---- //
	if (_robot->_dk_input.size() == 0) {
		printf("�v���C���[�����t�̂��߂�_dk_input�����������ĉ�����");
		return;
	}
	_robot->_dk_input[0]._dir = _robot->_pos + d_move;
	_robot->_dk_input[0]._length = 1;
	_robot->_dk_input[0]._rot_sum = rot_move;
}

void RobotCont::input(float delta_time) {
	// ----- �v���C���[�̑�����͏����i�[ ----- //
	// �O��
	if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) { _move.y = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) { _move.y = -1; }
	else { _move.y = 0; }
	// ���E
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) { _move.x = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) { _move.x = -1; }
	else { _move.x = 0; }
	// �}�E�X : �J�[�\���ʒu����ʒ����ɌŒ�A1�t���[���Ԃ̃}�E�X�ړ��ʂ�_mouse_input_st�Ɋi�[
	GetMousePoint(&_mouse_input_st.x_delta_move, &_mouse_input_st.y_delta_move);
	_mouse_input_st.x_delta_move = _mouse_input_st.x_delta_move - DXE_WINDOW_WIDTH / 2;
	_mouse_input_st.y_delta_move = _mouse_input_st.y_delta_move - DXE_WINDOW_HEIGHT / 2;
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPS�Ń}�E�X�ʒu����ʒ����ɌŒ�
}

int RobotCont::getAngleDir(const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z) {
	// ---- X,Z���ʏ��2�̃x�N�g��(�J�����̌��� <= ���{�b�g�̌���)�̉�]������0(����]), 1, -1�ŕԂ� ---- //
	tnl::Vector3 cam_dir_xz = { cam_dir_z.x, 0, cam_dir_z.z };
	cam_dir_xz.normalize();
	tnl::Vector3 rob_dir_xz = { rob_dir_z.x, 0, rob_dir_z.z };
	rob_dir_xz.normalize();
	tnl::Vector3 axis = { 0, 1, 0 };
	int dir = axis.dot(cam_dir_xz.cross(rob_dir_xz));
	float angle = tnl::Vector3::Dot(cam_dir_xz, rob_dir_xz);
	DrawStringEx(50, 85, -1, "%f", angle);
	if (1.0 - cos(tnl::ToRadian(89)) < angle) { return 0; }			// 1���̌덷�͋��e
	return - axis.dot(cam_dir_xz.cross(rob_dir_xz)) >= 0 ? 1 : -1;
}

