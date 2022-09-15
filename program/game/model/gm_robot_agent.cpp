#include "gm_robot_agent.h"

MdlRobotAgent_Rarms* MdlRobotAgent_Rarms::Create() {
	
	MdlRobotAgent_Rarms* agn = new MdlRobotAgent_Rarms();
	agn->modules_.resize(e_models_max);

	MdlRobotModule* shoulder = new MdlRobotModule();
	shoulder = shoulder->Create();
	agn->modules_[e_shoulder] = shoulder;

	MdlRobotModule* arm = new MdlRobotModule();
	arm = arm->Create();
	arm->pos_ += tnl::Vector3{ 0, 20, 0 };
	agn->modules_[e_arm] = arm;

	MdlRobotModule* wrist = new MdlRobotModule();
	wrist = wrist->Create();
	wrist->pos_ += tnl::Vector3{ 0, 40, 0 };
	agn->modules_[e_wrist] = wrist;

	return agn;
}

void MdlRobotAgent_Rarms::update(float delta_time) {
	for (auto mdl : modules_) {
		mdl->update(delta_time);
	}
}

void MdlRobotAgent_Rarms::render(dxe::Camera* camera) {
	for (auto mdl : modules_) {
		mdl->render(camera);
	}
}

void MdlRobotAgent_Rarms::directKinematics() {
	for (int i = 0; i < e_models_max; i++) {
		if (i == 0) {
			this->modules_[i]->localDK(this->rot_, this->oLi);
		}
		else if(i == e_models_max) {
			break;
		}
		else {
			tnl::Vector3 oLi = modules_[i - 1]->oLi_;
			tnl::Quaternion oqi = modules_[i - 1]->rot_;
			this->modules_[i]->localDK(oqi, oLi);
		}
	}
}

