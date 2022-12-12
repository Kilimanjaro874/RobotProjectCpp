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
		delete _robot;
	}
	//// ------ �����o�ϐ� ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.0f;			// �ړ����x(m/s)
	float _rot_speed = tnl::PI / 1;	// ��]���x(rad / s)

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
	// ---- �J�����ݒ� ---- 
	float _camera_distance = 20;			// ���{�b�g�ƃJ�����̋����w��
	float _camera_height = 10.0f;			// ���{�b�g���_(x, y=0 + this, z)����̃J���������_���w��

	//// ----- �����o�֐� ------ ////
	void update(float delta_time, GmCamera* _camera);
	void input(float delta_time);
	int getAngleDir(const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z);
};