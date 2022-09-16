#include "gm_ag_arm_r.h"

MdlArm_r* MdlArm_r::Create() {
	// ----- 管理するモジュール作成 ----- //
	// ---- 0. 自身：エージェント作成 ---- //
	MdlArm_r* agn = new MdlArm_r();
	agn->parts_.resize(e_models_max);
	// 位置・姿勢設定
	agn->pos_ = tnl::Vector3{ 0, 10, 0 };
	agn->rotAi_ = tnl::Vector3{ 0, 0, 1 };	

	// ---- 1. 肩モジュール作成 ---- //
	Module* shoulder = new Module();
	// 位置・姿勢設定
	shoulder->pos_ = tnl::Vector3{ 0, 10, 0 };
	shoulder->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	// --- パーツを登録していく ---
	// 1.0 : リンク
	Parts* link101 = new Parts();
	link101->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link101->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	link101->ofs_pos_ += shoulder->pos_ / 2;
	// 1.1 : 回転軸パーツ1
	Parts* axis101 = new Parts();
	axis101->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis101->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis101->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : 回転軸パーツ2
	Parts* axis102 = new Parts();
	axis102->mesh_ = dxe::Mesh::CreateDisk(5);
	axis102->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis102->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1,0,0 }, tnl::ToRadian(90));
	axis102->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : 回転軸パーツ3
	Parts* axis103 = new Parts();
	axis103->mesh_ = dxe::Mesh::CreateDisk(5);
	axis103->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis103->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	axis103->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- モジュールに登録 ---
	shoulder->parts_.push_back(link101);
	shoulder->parts_.push_back(axis101);
	shoulder->parts_.push_back(axis102);
	shoulder->parts_.push_back(axis103);
	// --- エージェントに登録 ---
	agn->modules_[e_shoulder] = shoulder;

	// ---- 2, 腕モジュール作成 ---- //
	

}

void MdlArm_r::update(float delta_time) {
	for (auto mdl : modules_) {
		mdl->update(delta_time);
	}
}

void MdlArm_r::render(dxe::Camera* camera) {
	for (auto mdl : modules_) {
		mdl->render(camera);
	}
}