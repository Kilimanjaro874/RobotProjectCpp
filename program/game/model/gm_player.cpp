#include "gm_player.h"
#include "gm_ag_arm_r.h"

Player* Player::Create() {
	// 試験的に作成
	Player* player = new Player();
	player->modules_.resize(e_modules_max);

	// 位置・姿勢設定
	player->pos_ = tnl::Vector3{ 0, 0, 0 };
	player->rotAi_ = tnl::Vector3{ 1, 0, 0 };
	player->forward_ = tnl::Vector3{ 1, 0, 0 };
	player->rot_ = tnl::Quaternion::RotationAxis(player->rotAi_, tnl::ToRadian(0));

	// エージェントクラス生成
	MdlArm_r* arm_r = nullptr;
	arm_r = MdlArm_r::Create();

	player->modules_[arm] = arm_r;

	return player;
}

void Player::move(float delta_time) {
	// 移動
	if(tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		delta_pos = forward_ * 10;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		delta_pos = -forward_ * 10;
	}

	/*if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(3));
		rot_sum_ *= rot_;
		forward_ = tnl::Vector3::TransformCoord(forward_, rot_);
	} else if(tnl::Input::IsKeyDown(eKeys::KB_LEFT)){
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(-3));
		rot_sum_ *= rot_;
		forward_ = tnl::Vector3::TransformCoord(forward_, rot_);
	}
	else {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(0));
		rot_sum_ *= rot_;
	}*/
	rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(5));
	tmpangle += 5;
}

void Player::update(float delta_time) {
	for (auto mdl : modules_) {
		mdl->update(delta_time);
	}
}

void Player::render(dxe::Camera* camera) {
	for (auto mdl : modules_) {
		mdl->render(camera);
	}
}

void Player::calcLDK() {
	// ----- 外部エージェントからq_back, l_backを貰い、保持モジュール全てでLDK計算実施する関数 ----- //




	if ((tmpangle) % 90 == 0) {
		printf("time");
	}

	// ---- エージェントのLDK実行 ----- //
	tnl::Vector3 l_back = { 0, 0, 0 };
	//tnl::Vector3 l_back = delta_pos;

	tnl::Quaternion q_back = rot_;
	tnl::Vector3 ans = q_back.getEuler() * 360 / 2 / tnl::PI;
	//this->localDirectKinematics(q_back, l_back);

	// --- 保持モジュールのLDK実行 ---
	for (auto mod : modules_) {
		//mod->calcLDK(q_back, l_back);
	}


	delta_pos = { 0, 0, 0 };
}