#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	Robot() {};
	~Robot() {
		for (int n = 0; n < modules_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
		for (int n = 0; n < targets_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
		for (int n = 0; n < objects_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
	}
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
	std::vector<dk_setting> rob_dk_s_v_;
	// ----- Render & DK & IK ------ //
	std::vector<std::vector<Module*>> modules_;
	std::vector<std::vector<Module*>> targets_;		// IKのターゲットを登録
	std::vector<std::vector<Module*>> objects_;		// IKの操作対象を登録
	// ----- ロボット移動パラメータ ----- //
	float move_speed_ = 50.0;		// 移動速度 : *= delta_time
	float rotate_speed_ = 1.5f;		// 旋回速度 : *= delta_time
	tnl::Vector3 d_move_;			// 1フレーム間のx, z移動量を格納
	tnl::Quaternion rot_move_;		// 1フレーム間の回転量を格納
	// ----- ロボット座標原点からルート座標への位置ベクトル ----- //

	// ----- メンバ関数 ------ //
	static Robot* Create(const tnl::Vector3& pos, const tnl::Quaternion& rot);

	// ---- mode01 : TPS射撃モード ---- //
	void mode01_init(float delta_time);
	void mode01_update(float delta_time);
	// --- キャラコン --- //
	void move(float delta_time);
	void moveDK();
	// --- ターゲットDK --- //
	void targetInit();
	void targetDK();
};