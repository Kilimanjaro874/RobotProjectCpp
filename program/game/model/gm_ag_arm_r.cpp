#include "gm_ag_arm_r.h"

MdlArm_r* MdlArm_r::Create() {
	// ----- 管理するモジュール作成 開始 ----- //
	// ---- 0. 自身：エージェント作成 ---- //
	MdlArm_r* agn = new MdlArm_r();
	agn->modules_.resize(e_models_max);
	// 位置・姿勢設定
	agn->pos_ = tnl::Vector3{ 0, 0, 0 };
	agn->rotAi_ = tnl::Vector3{ 0, 1, 0 };	
	agn->link_dir = tnl::Vector3{ 0, 1, 0 };
	agn->link_length = 20;


	// ---- 1. 肩モジュール作成 ---- //
	Module* shoulder = new Module();
	// 位置・姿勢設定
	shoulder->pos_ = tnl::Vector3{ 0, 0, 0 };
	shoulder->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	shoulder->link_dir = tnl::Vector3{ 0, 1, 0 };
	shoulder->link_length = 20;
	// --- パーツを登録していく ---
	// 1.0 : リンク
	Parts* link101 = new Parts();
	link101->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link101->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	link101->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
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
	axis102->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : 回転軸パーツ3
	Parts* axis103 = new Parts();
	axis103->mesh_ = dxe::Mesh::CreateDisk(5);
	axis103->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis103->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- モジュールに登録 ---
	shoulder->parts_.push_back(link101);
	shoulder->parts_.push_back(axis101);
	shoulder->parts_.push_back(axis102);
	shoulder->parts_.push_back(axis103);
	// --- エージェントに登録 ---
	agn->modules_[e_shoulder] = shoulder;


	// ---- 2, 腕モジュール作成 ---- //
	Module* arm = new Module();
	// 位置・姿勢設定
	arm->pos_ = tnl::Vector3{ 0, 20, 0 };
	arm->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	arm->link_dir = tnl::Vector3{ 0, 1, 0 };
	arm->link_length = 20;
	// --- パーツを登録していく ---
	// 1.0 : リンク
	Parts* link201 = new Parts();
	link201->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link201->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	link201->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
	// 1.1 : 回転軸パーツ1
	Parts* axis201 = new Parts();
	axis201->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis201->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis201->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : 回転軸パーツ2
	Parts* axis202 = new Parts();
	axis202->mesh_ = dxe::Mesh::CreateDisk(5);
	axis202->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis202->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : 回転軸パーツ3
	Parts* axis203 = new Parts();
	axis203->mesh_ = dxe::Mesh::CreateDisk(5);
	axis203->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis203->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- モジュールに登録 ---
	arm->parts_.push_back(link201);
	arm->parts_.push_back(axis201);
	arm->parts_.push_back(axis202);
	arm->parts_.push_back(axis203);
	// --- エージェントに登録 ---
	agn->modules_[e_arm] = arm;
	

	// ---- 3, 手首モジュール作成 ---- //
	Module* wrist = new Module();
	// 位置・姿勢設定
	wrist->pos_ = tnl::Vector3{ 0, 40, 0 };
	wrist->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	wrist->link_dir = tnl::Vector3{ 0, 1, 0 };
	wrist->link_length = 20;
	// --- パーツを登録していく ---
	// 1.0 : リンク
	Parts* link301 = new Parts();
	link301->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link301->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	link301->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
	// 1.1 : 回転軸パーツ1
	Parts* axis301 = new Parts();
	axis301->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis301->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis301->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : 回転軸パーツ2
	Parts* axis302 = new Parts();
	axis302->mesh_ = dxe::Mesh::CreateDisk(5);
	axis302->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis302->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : 回転軸パーツ3
	Parts* axis303 = new Parts();
	axis303->mesh_ = dxe::Mesh::CreateDisk(5);
	axis303->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// 位置・姿勢設定
	axis303->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- モジュールに登録 ---
	wrist->parts_.push_back(link301);
	wrist->parts_.push_back(axis301);
	wrist->parts_.push_back(axis302);
	wrist->parts_.push_back(axis303);
	// --- エージェントに登録 ---
	agn->modules_[e_wrist] = wrist;
	// ----- 管理するモジュール作成 終了 ----- //

	agn->update(0);	// 上記初期位置＆姿勢をゲーム描画前に適応させる。

	return agn;
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


void MdlArm_r::testmove() {

	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		pos_ = forward_ * 10;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		pos_ = -forward_ * 10;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		tempQ_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);

	} else if(tnl::Input::IsKeyDown(eKeys::KB_LEFT)){
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(-3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);
	}
	else {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(0));
	}

}


void MdlArm_r::calcLDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back, const tnl::Vector3& l_back) {
	// ----- 外部エージェントからq_back, l_backを貰い、保持モジュール全てでLDK計算実施する関数 ----- //
	
	// ---- エージェントのLDK実行 ----- //


	this->localDirectKinematics(p_back, q_back, l_back);
	

	// --- 保持モジュールのLDK実行 ---
	tnl::Vector3* tmp_p_back = &this->pos_;
	tnl::Quaternion* tmp_q_back = &this->rot_;
	tnl::Vector3* tmp_l_back = &this->xli_;
	for (auto mod : modules_) {
		mod->localDirectKinematics(*tmp_p_back, *tmp_q_back, *tmp_l_back);
		tmp_p_back = &mod->pos_next;
		tmp_q_back = &mod->rot_;
		tmp_l_back = &mod->xli_;
	}

	tempQ_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
}
