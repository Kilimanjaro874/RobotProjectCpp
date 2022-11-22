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
	//// ------ メンバ変数 ------ ////
	Robot* _robot = nullptr;
	float _move_speed = 10.f;			// 移動速度(m/s)
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

	//// ----- メンバ関数 ------ ////
	void update(float delta_time);
};