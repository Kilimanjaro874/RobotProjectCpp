#pragma once
#include "../game/model/gm_robot.h";

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
	float _move_speed = 10.f;			// �ړ����x(m/s)
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

	//// ----- �����o�֐� ------ ////
	void update(float delta_time);
};