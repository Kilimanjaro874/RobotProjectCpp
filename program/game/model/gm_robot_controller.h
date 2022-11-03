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
	//// ------ メンバ変数 ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.f;			// 移動速度(m/s)
	float _rot_speed = tnl::PI / 90;	// 回転速度(rad / s)

	//// ----- メンバ関数 ------ ////
	void update(float delta_time);
};