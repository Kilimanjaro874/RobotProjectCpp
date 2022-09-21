#pragma once
#include "gm_rb_arm_r.h"

class Robot : public Module {
	// ----- ���{�F�G�[�W�F���g�N���X�𓝊����郍�{�b�g�N���X���쐬 ----- //
public:
	
	Robot();
	~Robot() {
		delete arm_r;
	}

	// ---- �����o�ϐ� ---- //
	MdlRbArmR* arm_r = nullptr;

	


	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void move();

};