#include "gm_robot.h"

Robot::Robot() {
	// ---- 初期設定 ---- //
	pos_ = tnl::Vector3{ 0, 0, 0 };
	rotAi_ = tnl::Vector3{ 0, 1, 0 };
	link_dir = tnl::Vector3{ 0, 0, -1 };

	// ---- エージェントクラスを生成 ---- //
	arm_r = MdlRbArmR::Create(tnl::Vector3{ 0, 0, 0 }, tnl::Quaternion::RotationAxis(tnl::Vector3{ 0, 1, 0 }, 0));
	// ---- 目標位置格納 ---- //
	arm_r->pos_rs_.push_back(tnl::Vector3{ 0, 0, -200 });
	arm_r->pos_rs_.push_back(tnl::Vector3{ 50, 0, 0 });
	arm_r->pos_es_.push_back(arm_r->modules_[arm_r->e_wrist_z]->pos_next);
	arm_r->pos_es_.push_back(arm_r->modules_[arm_r->e_sho_z]->pos_next);
}

void Robot::update(float delta_time){

	arm_r->pos_es_.push_back(arm_r->modules_[arm_r->e_wrist_z]->pos_next);
	arm_r->pos_es_.push_back(arm_r->modules_[arm_r->e_sho_z]->pos_next);
	arm_r->update(delta_time);
}

void Robot::render(dxe::Camera* camera) {
	arm_r->render(camera);
}

void Robot::move() {
	// ---- ロボットの移動を受付 ---- //

}