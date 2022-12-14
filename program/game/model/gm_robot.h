#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	//// ------ メンバ変数 ------ ////
	// ----- ロボットの構成 ----- //
	enum {
		e_lower_back_,
		e_body_,
		e_r_leg,
		e_l_leg,
		e_head,
		e_r_arm,
		e_l_arm,
		e_modules_max
	};
	// ----- ワールド座標から見た本ロボットのDKパラメータ ----- //
	
	// ----- ロボット移動パラメータ ----- //
	float move_speed_ = 50.0;		// 移動速度 : *= delta_time
	float rotate_speed_ = 1.5f;		// 旋回速度 : *= delta_time
	tnl::Vector3 d_move_;			// 1フレーム間のx, z移動量を格納
	tnl::Quaternion rot_move_;		// 1フレーム間の回転量を格納
	std::vector<dk_st> _dk_input;	// コントローラーからのDK情報を格納
	// ----- ロボット速度制御パラメータ ----- //
	float mass = 500;				// 質量(kg)
	tnl::Vector3 vel_;				// 速度
	tnl::Vector3 acc_;				// 加速度
	// ----- csvからモジュールを構築するためのパラメータ ----- //
	
	


	//// ------ メンバ関数 ------- ////
	static Robot* Create(const tnl::Vector3 pos, const tnl::Quaternion rot);
	void init(Robot* rob, int id, std::string name, 
		tnl::Vector3 pos, tnl::Vector3 rot_axis, tnl::Quaternion rot);
	void move(float delta_time, const tnl::Vector3& pow, const tnl::Quaternion& rot_move, const tnl::Vector3& gravity);
	
	// ---- test ---- //
	// ---- CSVデータ読み込み関数 ---- //
	void getModuleDataCSV(Robot* rob, std::string csv_path);
	void getPartsDataCSV(Robot* rob, std::string csv_path);
	void getIKSetDataCSV(Robot* rob, std::string csv_path);
};