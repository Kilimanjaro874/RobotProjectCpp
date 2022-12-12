#include "gm_robot_controller.h"

void RobotCont::update(float delta_time, GmCamera* camera) {
	// ----- プレイヤー操作の影響をロボットクラスに与える ----- //
	// ---- 入力情報取得 ---- //
	input(delta_time);
	// ---- ロボット移動量初期化 ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- TPSカメラワーク ---- //
	camera->target_ = tnl::Vector3{ _robot->_pos.x, this->_camera_height, _robot->_pos.z };
	camera->target_distance_ = this->_camera_distance;
	DrawStringEx(50, 200, -1, "%2.5f, %2.5f, %2.5f", _robot->_pos.x, _robot->_pos.y, _robot->_pos.z);
	camera->free_look_angle_xy_ += tnl::Vector3{
		(float)_mouse_input_st.y_delta_move, 
		(float)_mouse_input_st.x_delta_move, 
		0
	} * delta_time * _camera_rot_coefficient;
	// ---- ロボット水平移動 ---- //
	d_move += _robot->_dir_z_tmp * delta_time * _move.y * _move_speed;
	d_move += _robot->_dir_x_tmp * delta_time * _move.x * _move_speed;
	// ---- ロボット回転処理(カメラの向きと同期) ---- //
	tnl::Vector3 cam_dir = camera->front();
	int angle_dir = getAngleDir(cam_dir, _robot->_dir_z_tmp);		// ロボットの回転方向決定(0 or 1 or -1)
	rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, angle_dir * _rot_speed * delta_time);
	

	// ---- ロボットの座標変換のためのパラメータ(水平移動、回転移動)を格納 ---- //
	if (_robot->_dk_input.size() == 0) {
		printf("プレイヤー操作受付のための_dk_inputを初期化して下さい");
		return;
	}
	_robot->_dk_input[0]._dir = _robot->_pos + d_move;
	_robot->_dk_input[0]._length = 1;
	_robot->_dk_input[0]._rot_sum = rot_move;
}

void RobotCont::input(float delta_time) {
	// ----- プレイヤーの操作入力情報を格納 ----- //
	// 前後
	if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) { _move.y = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) { _move.y = -1; }
	else { _move.y = 0; }
	// 左右
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) { _move.x = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) { _move.x = -1; }
	else { _move.x = 0; }
	// マウス : カーソル位置を画面中央に固定、1フレーム間のマウス移動量を_mouse_input_stに格納
	GetMousePoint(&_mouse_input_st.x_delta_move, &_mouse_input_st.y_delta_move);
	_mouse_input_st.x_delta_move = _mouse_input_st.x_delta_move - DXE_WINDOW_WIDTH / 2;
	_mouse_input_st.y_delta_move = _mouse_input_st.y_delta_move - DXE_WINDOW_HEIGHT / 2;
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPSでマウス位置を画面中央に固定
}

int RobotCont::getAngleDir(const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z) {
	// ---- X,Z平面上の2つのベクトル(カメラの向き <= ロボットの向き)の回転方向を0(無回転), 1, -1で返す ---- //
	tnl::Vector3 cam_dir_xz = { cam_dir_z.x, 0, cam_dir_z.z };
	cam_dir_xz.normalize();
	tnl::Vector3 rob_dir_xz = { rob_dir_z.x, 0, rob_dir_z.z };
	rob_dir_xz.normalize();
	tnl::Vector3 axis = { 0, 1, 0 };
	int dir = axis.dot(cam_dir_xz.cross(rob_dir_xz));
	float angle = tnl::Vector3::Dot(cam_dir_xz, rob_dir_xz);
	DrawStringEx(50, 85, -1, "%f", angle);
	if (1.0 - cos(tnl::ToRadian(89)) < angle) { return 0; }			// 1°の誤差は許容
	return - axis.dot(cam_dir_xz.cross(rob_dir_xz)) >= 0 ? 1 : -1;
}

