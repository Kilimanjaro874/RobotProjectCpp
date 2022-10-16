#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	// ---- Robot[id = 0] : パラメータ初期化 ---- //
	Robot* rob = new Robot();
	rob->InitParams(0, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// rob->SelfDK(pos, rot);				// 座標系の更新: 最後にまとめて実行すればよいのでは? 
	// DK パラメータ初期化
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 50, 0}, 1, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
	// モジュールサイズ設定
	rob->modules_.resize(e_modules_max);

	// ---- 1. lower back : パラメータ初期化
	rob->modules_[e_lower_back_].resize(3);		// 3DOF
	// --- 1.1. e_bb_y --- //
	Module* bb_y = new Module();
	// 子設定
	rob->next.resize(1);
	rob->next[0] = bb_y;
	// 親設定 
	bb_y->back = rob;
	// パラメータ設定
	bb_y->InitParams(100, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK パラメータ初期化
	bb_y->dk_s_v_.resize(1);
	bb_y->dk_s_v_[0] = { 101, {0, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_y->in_rot_axis_, 0) };

	// --- 1.2. e_bb_x --- //
	Module* bb_x = new Module();
	// 子設定
	bb_y->next.resize(1);
	bb_y->next[0] = bb_x;
	// 親設定
	bb_x->back = bb_y;
	// パラメータ設定
	bb_x->InitParams(101, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	// DK パラメータ初期化
	bb_x->dk_s_v_.resize(1);
	bb_x->dk_s_v_[0] = { 102, {0, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_x->in_rot_axis_, 0) };

	// --- 1.3. e_bb_z --- //
	Module* bb_z = new Module();
	// 子設定
	bb_x->next.resize(1);
	bb_x->next[0] = bb_z;
	// 親設定
	bb_z->back = bb_x;
	// パラメータ設定
	bb_z->InitParams(102, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	// DK パラメータ初期化
	bb_z->dk_s_v_.resize(4);	// body, r_leg, l_leg, object用
	bb_z->dk_s_v_[0] = { 200, {0, 25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[1] = { 300, {25, -25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[2] = { 400, {-25, -25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[3] = { 800, {25, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	// Parts
	Parts* bb_z_bone = new Parts();
	bb_z_bone->mesh_ = dxe::Mesh::CreateCylinder(5, 50);
	bb_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bb_z->parts_.push_back(bb_z_bone);
	

	// パーツの初期位置・姿勢を反映
	rob->rob_dk_s_v_.resize(1);
	tnl::Vector3 tmp_pos_dir = pos;
	tmp_pos_dir.normalize();
	float pos_length = pos.length();
	rob->rob_dk_s_v_[0] = {0, tmp_pos_dir, pos_length, rot};
	rob->AllInitDK(rob, rob->rob_dk_s_v_);

	return rob;
}

//void Robot::update(float delta_time) {
//	// ----- 各モジュールのupdate実行 ----- //
//	for (int n = 0; n < modules_.size(); n++) {
//		for (auto mod : modules_[n]) {
//			mod->update(delta_time);
//		}
//	}
//}
//
//void Robot::render(dxe::Camera* camera) {
//	// ----- 各モジュールのrender実行 ----- //
//	for (int n = 0; n < modules_.size(); n++) {
//		for (auto mod : modules_[n]) {
//			mod->render(camera);
//		}
//	}
//}

void Robot::mode01_init(float delta_time) {

}

void Robot::mode01_update(float delta_time) {

}

void Robot::move(float delta_time) {

}












