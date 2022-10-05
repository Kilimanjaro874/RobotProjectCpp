#include "gm_agent.h"

void Agent::update(float delta_time) {
	for (auto mod : modules_) {
		mod->update(delta_time);
	}
}

void Agent::render(dxe::Camera* camera) {
	for (auto mod : modules_) {
		mod->render(camera);
	}
}

//void Agent::mode01_init(FaceVec& target) {
//
//}

void Agent::mode01_update(float delta_time) {

}

void Agent::mode02_init(FaceVec& target) {

}

void Agent::mode02_update() {

}