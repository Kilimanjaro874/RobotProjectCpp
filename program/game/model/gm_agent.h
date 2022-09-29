#pragma once
#include "gm_module.h"

class Agent : public Module {
public:
	Agent() {};
	~Agent() {
		for (auto mod : modules_) delete mod;
	}
	std::vector<Module*> modules_;
	// ---- �G�[�W�F���g�ŋ��ʂ̍�Ƃ����X�ǉ��\�� ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
};