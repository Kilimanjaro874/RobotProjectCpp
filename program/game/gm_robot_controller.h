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
	//// ------ メンバ変数 ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.0f;			// 移動速度(m/s)
	float _rot_speed = tnl::PI / 1;	// 回転速度(rad / s)

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
	// ---- カメラ設定 ---- 
	float _camera_distance = 20;			// ロボットとカメラの距離指定
	float _camera_height = 10.0f;			// ロボット原点(x, y=0 + this, z)からのカメラ注視点を指定

	//// ----- メンバ関数 ------ ////
	void update(float delta_time, GmCamera* _camera);
	void input(float delta_time);
	int getAngleDir(const tnl::Vector3& cam_dir_z, const tnl::Vector3& rob_dir_z);
};