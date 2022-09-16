#include "gm_robot_module.h"

MdlRobotModule* MdlRobotModule::Create() {

	// ----- モジュールの初期設定 ----- //
	MdlRobotModule* mdl = new MdlRobotModule();
	mdl->parts_.resize(e_parts_max);

	// ----- モジュールのDKパラメータ初期化 ----- //
	mdl->rotAi_ = tnl::Vector3{ 0, 0, 1 };			// モジュールの回転軸ベクトル設定
	mdl->posLi_ = tnl::Vector3{ 0, 20, 0 };			// 本モジュールから次モジュールへの距離ベクトル

	// ---- モジュールのリンク01パーツ設定 ---- //
	Parts* link = new Parts();
	link->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- 初期位置＆姿勢変更 ---
	link->ofs_pos_ += mdl->posLi_ / 2;
	
	// --- モジュールに登録 --- 
	mdl->parts_[e_link01] = link;

	// ---- モジュールの回転軸01パーツ設定 ---- //
	Parts* axis01 = new Parts();
	axis01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- 初期位置＆姿勢変更 ---
	axis01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	
	// --- モジュールに登録 ---
	mdl->parts_[e_axis01] = axis01;

	// ---- モジュールの回転軸02パーツ設定 ---- //
	Parts* axis02 = new Parts();
	axis02->mesh_ = dxe::Mesh::CreateDisk(5);
	axis02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- 初期位置＆姿勢変更
	axis02->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	
	// --- モジュールに登録 --- 
	mdl->parts_[e_axis02] = axis02;

	// ---- モジュールの回転軸03パーツ設定 ---- //
	Parts* axis03 = new Parts();
	axis03->mesh_ = dxe::Mesh::CreateDisk(5);
	axis03->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- 初期位置＆姿勢変更
	axis02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	axis03->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };
	
	// --- モジュールに登録 --- 
	mdl->parts_[e_axis03] = axis03;

	return mdl;
}

void MdlRobotModule::update(float delta_time) {
	// ----- 基底クラス：モデルのアップデート ----- //
	//Model::update(delta_time);
	// ----- 本ロボットモジュール特有の処理記述 ----- //
	for (auto pts : parts_) {
		/*pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_);
		pts->mesh_->rot_q_ *= rot_;*/
		//pts->mesh_->rot_q_ *= pts->ofs_rot_;
	}
}

void MdlRobotModule::localDK(tnl::Quaternion q_back, tnl::Vector3 l_back) {
	// ----- 局所順運動実施 ----- //
	// ---- oΣi-1の情報で本モジュールの位置・回転軸ベクトル等の情報更新 ----
	pos_ = tnl::Vector3::TransformCoord(pos_, q_back) + l_back;
	
	
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// ---- テスト：iΣiの微小回転角度を作用させる ---- 
	float dth =	90;	// deg
	// ---- 局所的な(iΣi)回転クォータニオンiqi, 平行移動量iLi計算
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(dth));
	tnl::Vector3 iLi = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
	// ---- Σ0基準の回転クォータニオンoqi, 平行移動量oLi計算
	tnl::Quaternion oqi = iqi * q_back;
	rot_ = oqi;
	
	rot_total *= oqi;	// 回転量積算値
	
	tnl::Vector3 oLi = tnl::Vector3::TransformCoord(l_back, iqi) + iLi;
	oLi_ = oLi;
	// ---- 所持しているパーツの情報更新 ---- 
	for (auto pts : parts_) {
		
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_total);
		pts->mesh_->rot_q_ = pts->ofs_rot_* rot_total;
	}

}
