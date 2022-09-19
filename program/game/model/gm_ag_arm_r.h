#pragma once
#include "gm_module.h"

class MdlArm_r : public Module {
	// ----- ロボット：右腕を想定:モジュールをまとめあげるエージェントとして動作 ----- //
public:
	enum {
		e_shoulder,		// 肩
		e_arm,			// 腕
		e_wrist,		// 手首
		e_models_max	// おわり
	};
	std::vector<Module*>modules_;		// モジュールのベクター型
	std::vector<tnl::Vector3> pos_rs_;	// 各モジュールの手先制御目標位置を格納
	std::vector<tnl::Vector3> pos_es_;	// 各モジュールの手先制御目標位置へ追従させたい手先位置情報を格納
	tnl::Vector3 forward_;
	tnl::Quaternion tempQ_;
	tnl::Vector3 target_pos_;
	tnl::Vector3 target_es;

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ---- 各モジュールのLocalDirectKinematicsをエージェントとして実施 ---- //
	void calcLDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	void calcLDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// --- 各モジュールの目標位置・制御する手先位置のリスト作成 --- //
	



	void testmove();

	static MdlArm_r* Create(tnl::Vector3& target_pos);
};