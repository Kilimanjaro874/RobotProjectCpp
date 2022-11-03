#include "gm_robot_controller.h"

void RobotCont::update(float delta_time) {
	// ----- プレイヤー操作の影響をロボットクラスに与える ----- //
	
	// ---- 初期化 ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- 入力受付 ---- //
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
	// ---- ロボットの座標変換のためのパラメータを格納 ---- //
	if (_robot->_dk_input.size() == 0) {
		printf("プレイヤー操作受付のための_dk_inputを初期化して下さい"); 
		return; 
	}
	_robot->_dk_input[0]._dir = _robot->_pos + d_move;
	_robot->_dk_input[0]._length = 1;
	_robot->_dk_input[0]._rot_sum = rot_move;
	

}

