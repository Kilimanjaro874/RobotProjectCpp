#pragma once
#include "gm_module.h"

class MdlRbArmR : public Module {
public:

	MdlRbArmR() {};
	~MdlRbArmR() {
		for (auto mod : modules_) delete mod;
	}

	enum {
		e_sho_x,		// 肩：x軸モジュール
		e_sho_z,		// 肩：z軸
		e_arm_x,		// 腕
		e_arm_y,		
		e_wrist_x,		// 手首
		e_wrist_z,
		e_modules_max	// おわり
	};

	std::vector<Module*>modules_;		// モジュール参照型
	std::vector<tnl::Vector3> pos_rs_;	// 手先目標位置
	std::vector<tnl::Vector3> pos_es_;	// 制御対象の手先位置格納

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	static MdlRbArmR* Create(tnl::Vector3 p_back, tnl::Quaternion q_back);
	// ----- 試作：ターゲットに向かって腕でエイムする関数 ----- //
	void aimTarget(float delta_time, tnl:: Vector3 p_back, tnl::Quaternion q_back, tnl::Vector3 target, tnl::Vector3 elbowRef);
	
};