#include "gm_robot_controller.h"
#include "../game/model/gm_weapon.h"

void RobotCont::update(float delta_time, GmCamera* camera) {
	// ----- �v���C���[����̉e�������{�b�g�N���X�ɗ^���� ----- //
	// ---- ���{�b�g�ړ��ʏ����� ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- ���͏��擾 ---- //
	input(delta_time);
	// ---- TPS�J�������[�N ---- //
	cameraWorkTPS(delta_time, camera);
	// ---- ���{�b�g�����ړ� ---- //
	robMoveCont(delta_time);	
	//d_move += _robot->_dir_z_tmp * delta_time * _move.z * _move_speed;
	//d_move += _robot->_dir_x_tmp * delta_time * _move.x * _move_speed;
	// ---- ���{�b�g��]����(�J�����̌����Ɠ���) ---- //
	tnl::Vector3 cam_dir = camera->front();
	int angle_dir = getAngleDir(1.0, cam_dir, _robot->_dir_z_tmp);		// ���{�b�g�̉�]��������(0 or 1 or -1)
	rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, angle_dir * _rot_speed * delta_time);
	// ---- ���{�b�g�̍��W�ϊ��̂��߂̃p�����[�^(�����ړ��A��]�ړ�)���i�[ ---- //
	_robot->move(delta_time, _pow, rot_move, { 0, 0, 0 });
	/*if (_robot->_dk_input.size() == 0) {
		printf("�v���C���[�����t�̂��߂�_dk_input�����������ĉ�����");
		return;
	}
	_robot->_dk_input[0]._dir = _robot->_pos + d_move;
	_robot->_dk_input[0]._length = 1;
	_robot->_dk_input[0]._rot_sum = rot_move;*/
	// ---- �ȉ��A���[�h�ʂ̏��� ---- //
	if(_mode == aiming){
		// ---- �ˌ����[�h����update���� ---- //
		fireUpdate(delta_time, camera);
	}
}

void RobotCont::input(float delta_time) {
	// ----- �v���C���[�̑�����͏����i�[ ----- //
	// �O��
	if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) { _move.z = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) { _move.z = -1; }
	else { _move.z = 0; }
	// ���E
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) { _move.x = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) { _move.x = -1; }
	else { _move.x = 0; }
	// ��
	if (tnl::Input::IsKeyDown(eKeys::KB_SPACE)) { _move.y = 1; }
	else { _move.y = 0; }
	// �����F�O�㍶�E���͒lVector�̑傫����1�ȏ�F�@���K��
	if (_move.length() > 1.0) { _move.normalize(); }
	// �}�E�X : �J�[�\���ʒu����ʒ����ɌŒ�A1�t���[���Ԃ̃}�E�X�ړ��ʂ�_mouse_input_st�Ɋi�[
	GetMousePoint(&_mouse_input_st.x_delta_move, &_mouse_input_st.y_delta_move);
	_mouse_input_st.x_delta_move = _mouse_input_st.x_delta_move - DXE_WINDOW_WIDTH / 2;
	_mouse_input_st.y_delta_move = _mouse_input_st.y_delta_move - DXE_WINDOW_HEIGHT / 2;
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPS�Ń}�E�X�ʒu����ʒ����ɌŒ�
	// �Ί�ǐ�
	_is_weapon_l_fire = false;	// ���Z�b�g
	_is_weapon_r_fire = false;
	if (tnl::Input::IsMouseDown(tnl::Input::eMouse::LEFT)) { _is_weapon_l_fire = true; }
	if (tnl::Input::IsMouseDown(tnl::Input::eMouse::RIGHT)) {
		_is_weapon_r_fire = true; 
	}
}

int RobotCont::getAngleDir(float tolerance_deg, const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z) {
	// ---- X,Z���ʏ��2�̃x�N�g��(�J�����̌��� <= ���{�b�g�̌���)�̉�]������0(����]), 1, -1�ŕԂ� ---- //
	// tolerance_dig : 2�̃x�N�g���̋��e�p�x�덷(+- degree)
	tnl::Vector3 cam_dir_xz = { cam_dir_z.x, 0, cam_dir_z.z };
	cam_dir_xz.normalize();
	tnl::Vector3 rob_dir_xz = { rob_dir_z.x, 0, rob_dir_z.z };
	rob_dir_xz.normalize();
	tnl::Vector3 axis = { 0, 1, 0 };
	int dir = axis.dot(cam_dir_xz.cross(rob_dir_xz));
	float angle = tnl::Vector3::Dot(cam_dir_xz, rob_dir_xz);
	DrawStringEx(50, 85, -1, "%f", angle);
	if (1.0 - cos(tnl::ToRadian(90 - tolerance_deg)) < angle) { return 0; }			//���e�덷
	return - axis.dot(cam_dir_xz.cross(rob_dir_xz)) >= 0 ? 1 : -1;
}

tnl::Vector3 RobotCont::getAimPosition(const GmCamera* g_cam, tnl::Vector3 offset) {
	// ---- �J�����œ_�������Q�ƁE�G�C���œ_�ʒu��Ԃ� ---- //
	tnl::Vector3 aim_pos = g_cam->pos_ + g_cam->_focus_dir_tmp * _focal_length;
	return aim_pos;
}

void RobotCont::cameraWorkTPS(float delta_time, GmCamera* g_cam) {
	// ---- �}�E�X�ړ��ʂɉ����ăJ�����̌����ύX ---- //
	g_cam->target_ = tnl::Vector3{ _robot->_pos.x, this->_camera_height, _robot->_pos.z };
	g_cam->_target_distance = this->_camera_distance;
	DrawStringEx(50, 200, -1, "%2.5f, %2.5f, %2.5f", _robot->_pos.x, _robot->_pos.y, _robot->_pos.z);
	g_cam->_free_look_angle_xy += tnl::Vector3{
		(float)_mouse_input_st.y_delta_move,
		(float)_mouse_input_st.x_delta_move,
		0
	} *delta_time * _camera_rot_coefficient;

}

void RobotCont::fireUpdate(float delta_time, const GmCamera* g_cam) {
	// ---- �G�C�����ˌ����Ǘ�����֐� ---- //
	// --- ���r�̏��� --- //
	_aim_target_r = getAimPosition(g_cam);		// �E�r�̃G�C���^�[�Q�b�g�擾
	_aim_target_l = getAimPosition(g_cam);		// ���r�̃G�C���^�[�Q�b�g�擾
	_robot->TranlateTree(930, "", _aim_target_r, _robot->absolute);		// ��������������ID��\�ߎ擾���ׂ���
	_robot->setEffectIKTree(930, "", false);
	_robot->TranlateTree(940, "", _aim_target_l, _robot->absolute);
	_robot->setEffectIKTree(940, "", false);
	// --- �ˌ��̏��� --- //
	Weapon* r_weapon = static_cast<Weapon*>(_robot->getModulePointerTree(_robot, 800, ""));
	r_weapon->genBullet(delta_time, _is_weapon_r_fire);
	Weapon* l_weapon = static_cast<Weapon*>(_robot->getModulePointerTree(_robot, 850, ""));
	l_weapon->genBullet(delta_time, _is_weapon_l_fire);
	// --- �����̏��� --- //
	_head_target = getAimPosition(g_cam);		// ���̃G�C���^�[�Q�b�g�擾
	_robot->TranlateTree(951, "", _head_target, _robot->absolute);
	_robot->setEffectIKTree(951, "", false);
	// ---- ���{�b�g��DK&IK, ���iDK�X�V ---- //
	_robot->directKinematicsAndIKTree(_robot, _robot->_dk_input, delta_time);
	_robot->partsUpdateTree(_robot, delta_time);
}

void RobotCont::robMoveCont(float delta_time) {
	// ---- ���{�b�g�������������x����̓��͒l�F_acc_in�����肷�� ---- //
	if (delta_time == 0) { return; }	// �|�[�Y�����͎��{����
	// ������Z�b�g
	_vel_ref = { 0, 0, 0 };
	// ��������
	_vel_ref += _robot->_dir_z_tmp * _move.z * _horizontal_speed_lim;
	_vel_ref += _robot->_dir_x_tmp * _move.x * _horizontal_speed_lim;
	_vel_ref += tnl::Vector3{ 0, 1, 0 } *_move.y * _vertical_speed_lim;
	DrawStringEx(50, 50, -1, "vel_ref x=%f, z = %f", _vel_ref.x, _vel_ref.z);
	_vel_error = _vel_ref - _robot->vel_;
	DrawStringEx(50, 70, -1, "vel_error x=%f, z = %f", _vel_error.x, _vel_error.z);
	// PID���� : ������͒l����
	_vel_error_integral += (_vel_error + _vel_error_pre) / 2 * delta_time; // ���x�덷�ώZ�l
	_pow = _vel_error * _kp +
		_vel_error_integral * _ki +
		(_vel_error - _vel_error_pre) / delta_time;
	//_acc_in += pow / _mass;	// a = F/m�ŉ����x�w�ߒl����
	// ���i�[
	_vel_error_pre = _vel_ref;
}