#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	Robot() {};
	~Robot() {
		for (int n = 0; n < modules_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
	}
	// ----- ロボットの構成 ----- //
	enum {
		e_lower_back_,
		e_body_,
		e_r_arm,
		e_modules_max
	};

	// ----- Render & DK & IK ------ //
	std::vector<std::vector<Module*>> modules_;

	// ----- メンバ関数 ------ //
	static Robot* Create(const tnl::Vector3& pos, const tnl::Quaternion& rot);
	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ---- mode01 : TPS射撃モード ---- //
	void mode01_init(float delta_time);
	void mode01_update(float delta_time);
	// --- キャラコン --- //
	void move(float delta_time);
};