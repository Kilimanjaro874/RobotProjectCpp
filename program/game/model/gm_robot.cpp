#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	// ---- Robot[id = 0] : パラメータ初期化 ---- //
	Robot* rob = new Robot();
	rob->id_ = 0;
	rob->in_rot_axis_ = { 0, 1, 0 };
	rob->in_dir_z_ = { 0, 0, 1 };
	rob->in_dir_x_ = { 1, 0, 1 };
	rob->SelfDK(pos, rot);				// 座標系の更新 
	// DK パラメータ初期化
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 50, 0}, 0, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
	
	// ---- lower back : パラメータ初期化


	return rob;
}

void Robot::update(float delta_time) {
	// ----- 各モジュールのupdate実行 ----- //
	for (int n = 0; n < modules_.size(); n++) {
		for (auto mod : modules_[n]) {
			mod->update(delta_time);
		}
	}
}

void Robot::render(dxe::Camera* camera) {
	// ----- 各モジュールのrender実行 ----- //
	for (int n = 0; n < modules_.size(); n++) {
		for (auto mod : modules_[n]) {
			mod->render(camera);
		}
	}
}

void Robot::mode01_init(float delta_time) {

}

void Robot::mode01_update(float delta_time) {

}

void Robot::move(float delta_time) {

}










