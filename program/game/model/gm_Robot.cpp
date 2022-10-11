#include "gm_Robot.h"
#include "gm_agn_armR001.h"
#include "gm_agn_lowerBack001.h"

Robot* Robot::Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	Robot* rob = new Robot();
	rob->agents_.resize(e_agents_max);
	rob->init_rot_axis_ = tnl::Vector3{ 0, 1, 0 };
	rob->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	rob->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	rob->init_dir_r_ = tnl::Vector3{ 0, 1, 0 };
	rob->dir_z_ = rob->init_dir_z_;
	rob->dir_x_ = rob->init_dir_x_;
	rob->dir_r_ = rob->init_dir_r_;


	// DK test
	rob->dir_arm_r_ = tnl::Vector3{ 25, 40, 10 };
	rob->q_arm_r_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0);

	Agn_armR001* arm_r = Agn_armR001::Create(rob->dir_arm_r_, rob->q_arm_r_);
	rob->agents_[e_arm_r] = arm_r;

	rob->length_to_arm_r_ = rob->dir_arm_r_.length();
	rob->dir_arm_r_.normalize();
	rob->init_dir_arm_r_ = rob->dir_arm_r_;
	

	rob->dir_r_ = tnl::Vector3{ 0, 25, 0 };
	rob->length_to_arm_r_ = rob->dir_r_.length();
	rob->dir_r_.normalize();
	rob->init_dir_r_ = rob->dir_r_;
	rob->q_dir_r_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	Agn_lowerBack001* lb = Agn_lowerBack001::Create(rob->init_dir_r_ * rob->length_to_root, rob->q_dir_r_);
	rob->agents_[e_lower_back] = lb;
	

	return rob;
}

void Robot::update(float delta_time) {
	for (auto agn : agents_) {
		agn->update(delta_time);
	}
	move(delta_time);

	// mode 01 update
	for (int i = 0; i < targets_.size(); i++) {
		targets_[i]->DirectKinematics(pos_o_, rot_tmp_);
		targets_[i]->update(delta_time);
	}

	// to armR para update
	dir_arm_r_ = tnl::Vector3::TransformCoord(init_dir_arm_r_, rot_sum_);
	dir_arm_r_ = dir_arm_r_ * length_to_arm_r_ + pos_o_;

	//agents_[e_arm_r]->mode01_update(delta_time, dir_arm_r_, rot_tmp_);
	//mode01_update(delta_time, dir_arm_r_, rot_tmp_, targets_);
	agents_[e_arm_r]->mode01_update(delta_time, dir_arm_r_, rot_tmp_, targets_);

	agents_[e_lower_back]->mode01_update(delta_time, init_dir_arm_r_ * length_to_root, q_dir_r_, targets_);

}

void Robot::move(float delta_time) {
	// 初期化
	rot_tmp_ = tnl::Quaternion::RotationAxis(init_rot_axis_, 0);
	d_move_ = { 0, 0, 0 };
	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		d_move_ = dir_z_ * delta_time * move_speed_;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		d_move_ -= dir_z_ * delta_time * move_speed_;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_tmp_ = tnl::Quaternion::RotationAxis(init_rot_axis_, rotate_speed_ * delta_time);
		rot_sum_ *= rot_tmp_;
		dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		rot_tmp_ = tnl::Quaternion::RotationAxis(init_rot_axis_, -rotate_speed_ * delta_time);
		rot_sum_ *= rot_tmp_;
		dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	}

	// robo DK
	pos_o_ += d_move_;

}



void Robot::render(dxe::Camera* camera) {
	// デバッグ
	DrawOBB(camera, pos_o_, rot_sum_, { 50, 50, 20 });
	for (auto agn : agents_) {
		agn->render(camera);
	}
}

void Robot::mode01_init(const std::vector<FaceVec*> targets) {

	targets_.resize(2);
	targets_[0] = targets[0];
	targets_[1] = targets[1];


	for (auto agn : agents_) {
		agn->mode01_init(targets);
	}

}

void Robot::mode01_update(float delta_time, const tnl::Vector3& p_back, 
	const tnl::Quaternion& q_back, std::vector<FaceVec*> targets) {

	for (auto agn : agents_) {
		agn->mode01_update(delta_time, p_back, q_back, targets_);
	}



}

void Robot::mode02_init(FaceVec& target) {

}

void Robot::mode02_update() {

}

