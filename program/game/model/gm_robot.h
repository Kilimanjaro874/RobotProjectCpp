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
		e_r_leg_,
		e_l_leg_,
		e_head,
		e_r_arm,
		e_l_arm
	};

	// ----- Render & DK & IK ------ //
	std::vector<std::vector<Module*>> modules_;

	// ----- メンバ関数 ------ //
	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void move(float delta_time);
	void mode01_init(float delta_time);
	void mode01_update(float delta_time);

};