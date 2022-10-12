#pragma once
#include "gm_rb_arm_r.h"

class Robot : public Module {
	// ----- ロボ：エージェントクラスを統括するロボットクラスを作成 ----- //
public:
	
	Robot();
	~Robot() {
		delete arm_r;
	}

	// ---- メンバ変数 ---- //
	MdlRbArmR* arm_r = nullptr;

	


	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void move();

};