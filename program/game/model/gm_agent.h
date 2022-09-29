#pragma once
#include "gm_module.h"

class Agent : public Module {
public:
	Agent() {};
	~Agent() {
		for (auto mod : modules_) delete mod;
	}
	std::vector<Module*> modules_;
	// ---- エージェントで共通の作業を次々追加予定 ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
};