#pragma once
#include "../game/model/gm_robot.h";
#include "../game/gm_camera.h"

class RobotCont {
public:
	RobotCont(Robot* robot) {
		_robot = robot; 
		_dk_st_next._id = robot->_id;
		_dk_st_next._name = robot->_name;
	};
	~RobotCont() {
		
	}
	//// ------ �����o�ϐ� ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.0f;			// �ړ����x(m/s)
	float _rot_speed = tnl::PI / 1;	// ��]���x(rad / s)
	// ----- ���{�b�g�̓��샂�[�h���` ----- //
	enum move_mode {
		normal, aiming
	};
	move_mode _mode = aiming;			// ������ԁF�G�C�����
	// ----- ����̉e�����^���w�v�Z�ɋL�q�����{�b�g�ɓn���A���̂��߂̍\���� ----- //
	struct dk_st {
		// ---- �e����q���W���[���։^���w�v�Z�����{���邽�߂̃p�����[�^���\���̂Œ�`���Ă��� ---- //
		int _id;						// �q���W���[��id
		std::string _name;				// �q���W���[��name
		tnl::Vector3 _dir;				// �q���W���[���̕�����P�ʃx�N�g���Ŋi�[(��o)
		float _length;					// �q���W���[���̋�����float�Ŋi�[(��o)
		tnl::Quaternion _rot_sum;		// �e���W���[������̉�]�ʑ��ʂ��i�[����(��o)
	};
	dk_st _dk_st_next;
	// ---- ������͎�t ----
	tnl::Vector3 _move;						// �����ړ��ʂ��{�^������擾
	struct mouse_inputs{					// 1�t���[���Ԃ̃}�E�X�ψʗʊi�[
		int x_delta_move;
		int y_delta_move;
	} _mouse_input_st;
	float _camera_rot_coefficient = 0.2f;	// �J��������}�E�X���x
	// ---- �J�����ݒ� ---- //
	float _camera_distance = 20;			// ���{�b�g�ƃJ�����̋����w��
	float _camera_height = 12.0f;			// ���{�b�g���_(x, y=0 + this, z)����̃J���������_���w��
	// ---- �G�C���֌W ---- //
	float _focal_length = 500;				// �G�C���œ_����
	tnl::Vector3 _aim_target_r;				// �E�r�̃G�C���Ώۍ��W���i�[
	tnl::Vector3 _aim_target_l;				// ���r�̃G�C���Ώۍ��W���i�[
	int _aim_target_r_id;					// �E�r�̃G�C���^�[�Q�b�gID���i�[
	int _aim_target_l_id;					// ���r�̃G�C���^�[�Q�b�gID���i�[
	// ---- ���{�b�g����p�p�����[�^ ---- //
	
	tnl::Vector3 _vel_cont_target;	// ���x�ڕW
	tnl::Vector3 _vel_lim_normal;
	tnl::Vector3 _vel_lim_qBoost;
	tnl::Vector3 _thrust_pow;
	tnl::Vector3 _vel_ref;				// �ڕW���x
	tnl::Vector3 _vel_error;			// �ڕW���x�Ƃ̃G���[
	tnl::Vector3 _vel_error_pre;		// �ڕW���x�Ƃ̃G���[(1�t���[���O)
	tnl::Vector3 _acc_in;				// �����x���͒l
	tnl::Vector3 _vel_error_integral;	// ���x�덷�ώZ�l
		
	float _mass = 1000;							// ���{�b�g����(kg)
	float _horizontal_speed_lim = 27.8;			// �������x���E(m/s)
	float _vertical_speed_lim = 10.0;			// ����������x���E(m/s)
	float _kp = 15.0;
	float _ki = 0.005;
	float _kd = 0.2;
	

	//// ----- �����o�֐� ------ ////
	void update(float delta_time, GmCamera* _camera);
	void input(float delta_time);
	void aimingUpdate(float delta_time, GmCamera);
	int getAngleDir(float tolerance_deg, const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z);
	tnl::Vector3 getAimPosition(const GmCamera* g_cam, tnl::Vector3 offset = { 0, 0, 0 });
	void cameraWorkTPS(float delta_time, GmCamera* g_cam);
	void fireUpdate(float delta_time, const GmCamera* g_cam);
	void robMoveCont(float delta_time);
};	