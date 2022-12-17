#include "gm_robot_controller.h"
#include "../game/model/gm_weapon.h"

void RobotCont::update(float delta_time, GmCamera* camera) {
	// ----- プレイヤー操作の影響をロボットクラスに与える ----- //
	// ---- ロボット移動量初期化 ---- //
	tnl::Quaternion rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 d_move = tnl::Vector3{ 0, 0, 0 };
	// ---- 入力情報取得 ---- //
	input(delta_time);
	// ---- TPSカメラワーク ---- //
	cameraWorkTPS(delta_time, camera);
	// ---- ロボット水平移動 ---- //
	robMoveCont(delta_time);	
	//d_move += _robot->_dir_z_tmp * delta_time * _move.z * _move_speed;
	//d_move += _robot->_dir_x_tmp * delta_time * _move.x * _move_speed;
	// ---- ロボット回転処理(カメラの向きと同期) ---- //
	tnl::Vector3 cam_dir = camera->front();
	int angle_dir = getAngleDir(1.0, cam_dir, _robot->_dir_z_tmp);		// ロボットの回転方向決定(0 or 1 or -1)
	rot_move = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, angle_dir * _rot_speed * delta_time);
	// ---- ロボットの座標変換のためのパラメータ(水平移動、回転移動)を格納 ---- //
	_robot->move(delta_time, _pow, rot_move, { 0, 0, 0 });
	/*if (_robot->_dk_input.size() == 0) {
		printf("プレイヤー操作受付のための_dk_inputを初期化して下さい");
		return;
	}
	_robot->_dk_input[0]._dir = _robot->_pos + d_move;
	_robot->_dk_input[0]._length = 1;
	_robot->_dk_input[0]._rot_sum = rot_move;*/
	// ---- 以下、モード別の処理 ---- //
	if(_mode == aiming){
		// ---- 射撃モード時のupdate処理 ---- //
		fireUpdate(delta_time, camera);
	}
}

void RobotCont::input(float delta_time) {
	// ----- プレイヤーの操作入力情報を格納 ----- //
	// 前後
	if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) { _move.z = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) { _move.z = -1; }
	else { _move.z = 0; }
	// 左右
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) { _move.x = 1; }
	else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) { _move.x = -1; }
	else { _move.x = 0; }
	// 上
	if (tnl::Input::IsKeyDown(eKeys::KB_SPACE)) { _move.y = 1; }
	else { _move.y = 0; }
	// 処理：前後左右入力値Vectorの大きさが1以上：　正規化
	if (_move.length() > 1.0) { _move.normalize(); }
	// マウス : カーソル位置を画面中央に固定、1フレーム間のマウス移動量を_mouse_input_stに格納
	GetMousePoint(&_mouse_input_st.x_delta_move, &_mouse_input_st.y_delta_move);
	_mouse_input_st.x_delta_move = _mouse_input_st.x_delta_move - DXE_WINDOW_WIDTH / 2;
	_mouse_input_st.y_delta_move = _mouse_input_st.y_delta_move - DXE_WINDOW_HEIGHT / 2;
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPSでマウス位置を画面中央に固定
	// 火器管制
	_is_weapon_l_fire = false;	// リセット
	_is_weapon_r_fire = false;
	if (tnl::Input::IsMouseDown(tnl::Input::eMouse::LEFT)) { _is_weapon_l_fire = true; }
	if (tnl::Input::IsMouseDown(tnl::Input::eMouse::RIGHT)) {
		_is_weapon_r_fire = true; 
	}
}

int RobotCont::getAngleDir(float tolerance_deg, const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z) {
	// ---- X,Z平面上の2つのベクトル(カメラの向き <= ロボットの向き)の回転方向を0(無回転), 1, -1で返す ---- //
	// tolerance_dig : 2つのベクトルの許容角度誤差(+- degree)
	tnl::Vector3 cam_dir_xz = { cam_dir_z.x, 0, cam_dir_z.z };
	cam_dir_xz.normalize();
	tnl::Vector3 rob_dir_xz = { rob_dir_z.x, 0, rob_dir_z.z };
	rob_dir_xz.normalize();
	tnl::Vector3 axis = { 0, 1, 0 };
	int dir = axis.dot(cam_dir_xz.cross(rob_dir_xz));
	float angle = tnl::Vector3::Dot(cam_dir_xz, rob_dir_xz);
	DrawStringEx(50, 85, -1, "%f", angle);
	if (1.0 - cos(tnl::ToRadian(90 - tolerance_deg)) < angle) { return 0; }			//許容誤差
	return - axis.dot(cam_dir_xz.cross(rob_dir_xz)) >= 0 ? 1 : -1;
}

tnl::Vector3 RobotCont::getAimPosition(const GmCamera* g_cam, tnl::Vector3 offset) {
	// ---- カメラ焦点方向を参照・エイム焦点位置を返す ---- //
	tnl::Vector3 aim_pos = g_cam->pos_ + g_cam->_focus_dir_tmp * _focal_length;
	return aim_pos;
}

void RobotCont::cameraWorkTPS(float delta_time, GmCamera* g_cam) {
	// ---- マウス移動量に応じてカメラの向き変更 ---- //
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
	// ---- エイム＆射撃を管理する関数 ---- //
	// --- 両腕の処理 --- //
	_aim_target_r = getAimPosition(g_cam);		// 右腕のエイムターゲット取得
	_aim_target_l = getAimPosition(g_cam);		// 左腕のエイムターゲット取得
	_robot->TranlateTree(930, "", _aim_target_r, _robot->absolute);		// 初期化処理等でIDを予め取得すべきだ
	_robot->setEffectIKTree(930, "", false);
	_robot->TranlateTree(940, "", _aim_target_l, _robot->absolute);
	_robot->setEffectIKTree(940, "", false);
	// --- 射撃の処理 --- //
	Weapon* r_weapon = static_cast<Weapon*>(_robot->getModulePointerTree(_robot, 800, ""));
	r_weapon->genBullet(delta_time, _is_weapon_r_fire);
	Weapon* l_weapon = static_cast<Weapon*>(_robot->getModulePointerTree(_robot, 850, ""));
	l_weapon->genBullet(delta_time, _is_weapon_l_fire);
	// --- 頭部の処理 --- //
	_head_target = getAimPosition(g_cam);		// 頭のエイムターゲット取得
	_robot->TranlateTree(951, "", _head_target, _robot->absolute);
	_robot->setEffectIKTree(951, "", false);
	// ---- ロボットのDK&IK, 部品DK更新 ---- //
	_robot->directKinematicsAndIKTree(_robot, _robot->_dk_input, delta_time);
	_robot->partsUpdateTree(_robot, delta_time);
}

void RobotCont::robMoveCont(float delta_time) {
	// ---- ロボット水平＆垂直速度制御の入力値：_acc_inを決定する ---- //
	if (delta_time == 0) { return; }	// ポーズ中等は実施無し
	// 上方リセット
	_vel_ref = { 0, 0, 0 };
	// 水平方向
	_vel_ref += _robot->_dir_z_tmp * _move.z * _horizontal_speed_lim;
	_vel_ref += _robot->_dir_x_tmp * _move.x * _horizontal_speed_lim;
	_vel_ref += tnl::Vector3{ 0, 1, 0 } *_move.y * _vertical_speed_lim;
	DrawStringEx(50, 50, -1, "vel_ref x=%f, z = %f", _vel_ref.x, _vel_ref.z);
	_vel_error = _vel_ref - _robot->vel_;
	DrawStringEx(50, 70, -1, "vel_error x=%f, z = %f", _vel_error.x, _vel_error.z);
	// PID制御 : 制御入力値決定
	_vel_error_integral += (_vel_error + _vel_error_pre) / 2 * delta_time; // 速度誤差積算値
	_pow = _vel_error * _kp +
		_vel_error_integral * _ki +
		(_vel_error - _vel_error_pre) / delta_time;
	//_acc_in += pow / _mass;	// a = F/mで加速度指令値決定
	// 情報格納
	_vel_error_pre = _vel_ref;
}