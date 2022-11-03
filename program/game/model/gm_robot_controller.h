#pragma once
#include "gm_robot.h";

class RobotCont {
public:
	RobotCont(Robot* robot) {
		_robot = robot; 
	};
	~RobotCont() {
		delete _robot;
	}
	//// ------ �����o�ϐ� ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.f;			// �ړ����x(m/s)
	float _rot_speed = tnl::PI / 90;	// ��]���x(rad / s)

	//// ----- �����o�֐� ------ ////
	void update(float delta_time);
};