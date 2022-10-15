#include "gm_robot.h"

void Robot::update(float delta_time) {
	for (int n = 0; n < modules_.size(); n++) {
		for (auto mod : modules_[n]) {
			mod->update(delta_time);
		}
	}
}

void Robot::render(dxe::Camera* camera) {
	for (int n = 0; n < modules_.size(); n++) {
		for (auto mod : modules_[n]) {
			mod->render(camera);
		}
	}
}



