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
	//// ------ メンバ変数 ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.0f;			// 移動速度(m/s)
	float _rot_speed = tnl::PI / 1;	// 回転速度(rad / s)
	// ----- ロボットの動作モードを定義 ----- //
	enum move_mode {
		normal, aiming
	};
	move_mode _mode = aiming;			// 初期状態：エイム状態
	// ----- 操作の影響を運動学計算に記述＆ロボットに渡す、そのための構造体 ----- //
	struct dk_st {
		// ---- 親から子モジュールへ運動学計算を実施するためのパラメータを構造体で定義しておく ---- //
		int _id;						// 子モジュールid
		std::string _name;				// 子モジュールname
		tnl::Vector3 _dir;				// 子モジュールの方向を単位ベクトルで格納(Σo)
		float _length;					// 子モジュールの距離をfloatで格納(Σo)
		tnl::Quaternion _rot_sum;		// 親モジュールからの回転量総量を格納する(Σo)
	};
	dk_st _dk_st_next;
	// ---- 操作入力受付 ----
	tnl::Vector3 _move;						// 水平移動量をボタンから取得
	struct mouse_inputs{					// 1フレーム間のマウス変位量格納
		int x_delta_move;
		int y_delta_move;
	} _mouse_input_st;
	float _camera_rot_coefficient = 0.2f;	// カメラ動作マウス感度
	// ---- カメラ設定 ---- //
	float _camera_distance = 20;			// ロボットとカメラの距離指定
	float _camera_height = 12.0f;			// ロボット原点(x, y=0 + this, z)からのカメラ注視点を指定
	// ---- エイム関係 ---- //
	float _focal_length = 500;				// エイム焦点距離
	tnl::Vector3 _aim_target_r;				// 右腕のエイム対象座標を格納
	tnl::Vector3 _aim_target_l;				// 左腕のエイム対象座標を格納
	int _aim_target_r_id;					// 右腕のエイムターゲットIDを格納
	int _aim_target_l_id;					// 左腕のエイムターゲットIDを格納
	//// ----- メンバ関数 ------ ////
	void update(float delta_time, GmCamera* _camera);
	void input(float delta_time);
	void aimingUpdate(float delta_time, GmCamera);
	int getAngleDir(float tolerance_deg, const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z);
	tnl::Vector3 getAimPosition(const GmCamera* g_cam, tnl::Vector3 offset = { 0, 0, 0 });
	void cameraWorkTPS(float delta_time, GmCamera* g_cam);
	void fireUpdate(float delta_time, const GmCamera* g_cam);
};	