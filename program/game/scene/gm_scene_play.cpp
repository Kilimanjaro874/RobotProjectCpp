#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

#include "../model/gm_rb_arm_r.h"


tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {

}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();

	arm_r_ = MdlRbArmR::Create(pos_, rot_);
	arm_r_->update(0);
	arm_r_->render(camera_);
	// ターゲットの設定
	arm_r_->pos_rs_.push_back(targetPos_);
	arm_r_->pos_rs_.push_back(elbowRefPos_);

	// ターゲットの可視化
	target_View_ = new Parts();
	target_View_->mesh_ = dxe::Mesh::CreateSphere(8);
	target_View_->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	target_View_->mesh_->pos_ = targetPos_;

	elbowTar_View_ = new Parts();
	elbowTar_View_->mesh_ = dxe::Mesh::CreateSphere(5);
	elbowTar_View_->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	elbowTar_View_->mesh_->pos_ = elbowRefPos_;
	

}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	
	//------------------------------------------------------------------
	//
	// カメラ制御
	//
	tnl::Vector3 rot[4] = {
		{ 0, tnl::ToRadian(1.0f), 0 },
		{ 0, -tnl::ToRadian(1.0f), 0 },
		{ tnl::ToRadian(1.0f), 0, 0 },
		{ -tnl::ToRadian(1.0f), 0, 0 } };
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		camera_->free_look_angle_xy_ += rot[idx];
	}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);

	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		camera_->target_distance_ += 1.0f;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_X)) {
		camera_->target_distance_ -= 1.0f;

	}	
	
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}
	

	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		origin_ += forward_ * 10;
		

	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		origin_ -= forward_ * 10;
		
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(3));
		rot_sum_ *= rot_;
		
		forward_ = tnl::Vector3::TransformCoord(forward_, rot_);

	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(-3));
		rot_sum_ *= rot_;
		forward_ = tnl::Vector3::TransformCoord(forward_, rot_);
	}
	else {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(0));
	}

	// プレイヤーの座標系更新
	pos_dir_ = tnl::Vector3::TransformCoord(pos_dir_, rot_);
	pos_dir_.normalize();
	pos_ = origin_ + pos_dir_ * pos_length;
	el_dir_ = tnl::Vector3::TransformCoord(el_dir_, rot_);
	el_dir_.normalize();
	elbowRefPos_ = origin_ + el_dir_ * el_length;
	
	// 目標位置更新
	arm_r_->pos_rs_.clear();
	arm_r_->pos_rs_.push_back(targetPos_);
	arm_r_->pos_rs_.push_back(elbowRefPos_);
	
	
	/*arm_r_->calcLDKwithLIK(delta_time, tmp_pos, tmp_q_back, arm_r_->target_es, arm_r_->target_pos_);
	arm_r_->update(delta_time);*/

	/*for (int i = 0; i < arm_r_->modules_.size(); i++) {
		arm_r_->pos_es_[i] = arm_r_->target_es;
	}*/

	// ターゲットの設定
	tnl::Vector3 tmp_pos = pos_;
	tnl::Quaternion tmp_q_back = rot_;

	arm_r_->aimTarget(delta_time, pos_, rot_, targetPos_, elbowRefPos_);
	arm_r_->update(delta_time);
	
}

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(50, 20);

	DrawOBB(origin_, rot_sum_, { 60, 100, 30 });
	DrawOBB(origin_ + forward_ * 60, rot_sum_, { 10, 10, 10 });

	arm_r_->render(camera_);

	// ターゲットの表示
	target_View_->mesh_->render(camera_);
	elbowTar_View_->mesh_->pos_ = elbowRefPos_;
	elbowTar_View_->mesh_->render(camera_);

}
