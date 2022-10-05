#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	delete elbow_r_tar;
	delete aim_tar_;
	delete arm_r;
	delete effector_obj_;
	delete elbow_obj_;
}


void ScenePlay::initialzie() {

	camera_ = new GmCamera();
	camera_->pos_ = { 100, 0, -300 };

	// ---- 試験 ----- //
	
	// --- 照準位置 --- //
	aim_tar_ = FaceVec::Create({0, 0, 70});
	aim_tar_->Rotate(tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(45)));
	aim_tar_->update(0);
	// --- 肘位置 --- //
	elbow_r_tar = FaceVec::Create({ 30, -30, 0 });
	elbow_r_tar->Rotate(tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90)));
	//elbow_r_tar->Rotate(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90)));
	elbow_r_tar->update(0);
	// --- 目標位置・姿勢格納 ---
	targets_.clear();
	targets_.push_back(aim_tar_);
	targets_.push_back(elbow_r_tar);

	// --- 制御対象の生成 --- //
	effector_obj_ = FaceVec::Create({ 0, 0, 0 });
	elbow_obj_ = FaceVec::Create({ 0, 0, 0 });
	objects_.clear();
	objects_.push_back(effector_obj_);
	objects_.push_back(elbow_obj_);

	// --- 腕の作成 ---d
	arm_r = Agn_armR001::Create(tnl::Vector3{ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	arm_r->update(0);

	// --- 試験：arm_rの構え姿勢 --- //
	arm_r->cnt_objects_ = objects_;
	arm_r->mode01_init(targets_);
	
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
	// --- シーン切り替え --- //
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	// ---- 一般形Agent試作＆運用 --- //
	arm_r->mode01_update(delta_time);
	arm_r->update(delta_time);

	// --------------------------------------試作開始 ----------------------------------
	//// -- 手先現在位置の取得 -- //
	//aim_obj_->pos_ = arm_r->modules_[Agn_armR001::e_wrist_z2]->pos_o_next_;
	////aim_obj_->rot_sum_ = arm_r->modules_[Agn_armR001::e_wrist_z2]->rot_sum_;
	//aim_obj_->Rotate(arm_r->modules_[Agn_armR001::e_wrist_z2]->rot_tmp_);
	//aim_obj_->update(delta_time);
	//DrawStringEx(10, 10, -1, "E.E. pos: x=%f, y=%f, z=%f", aim_obj_->pos_.x, aim_obj_->pos_.y, aim_obj_->pos_.z);
	//DrawStringEx(10, 25, -1, "E.E. drz: x=%f, y=%f, z=%f", aim_obj_->dir_z_.x, aim_obj_->dir_z_.y, aim_obj_->dir_z_.z);
	//DrawStringEx(10, 40, -1, "E.E. drx: x=%f, y=%f, z=%f", aim_obj_->dir_x_.x, aim_obj_->dir_x_.y, aim_obj_->dir_x_.z);

	//// --- IK＆DKを一歩ずつ解いていく＆検算していく --- //
	//tnl::Vector3 tmp_pos = { 0, 0, 0 };
	//tnl::Quaternion tmp_q = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0);
	//// -- agentのDK
	//arm_r->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->pos_o_next_;
	//tmp_q = arm_r->rot_tmp_;
	//DrawStringEx(10, 55, -1, "angAxis: x=%f, y=%f, z=%f", arm_r->rot_axis_.x, arm_r->rot_axis_.y, arm_r->rot_axis_.z);
	//
	//// --1.e_sho_x -- //
	//arm_r->modules_[Agn_armR001::e_sho_x]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_sho_x]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_sho_x]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_sho_x]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_sho_x]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_sho_x]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_sho_x]->rot_tmp_;
	tnl::Vector3 debAxis = arm_r->modules_[Agn_armR001::e_sho_x]->rot_axis_;
	tnl::Vector3 debDirz = arm_r->modules_[Agn_armR001::e_sho_x]->dir_z_;
	tnl::Vector3 debDirx = arm_r->modules_[Agn_armR001::e_sho_x]->dir_x_;
	DrawStringEx(10, 70, -1,	"1:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 85, -1,	"1:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 100, -1,	"1:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --2.e_sho_z -- //
	//arm_r->modules_[Agn_armR001::e_sho_y]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_sho_y]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_sho_y]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_sho_y]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_sho_y]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_sho_y]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_sho_y]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_sho_y]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_sho_y]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_sho_y]->dir_x_;
	DrawStringEx(10, 115, -1,	"2:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 130, -1,	"2:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 145, -1,	"2:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --3.e_arm_x -- //
	//arm_r->modules_[Agn_armR001::e_arm_x]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_arm_x]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_arm_x]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_arm_x]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_arm_x]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_arm_x]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_arm_x]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_arm_x]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_arm_x]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_arm_x]->dir_x_;
	DrawStringEx(10, 160, -1, "3:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 175, -1, "3:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 190, -1, "3:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --4.e_arm_z -- //
	//arm_r->modules_[Agn_armR001::e_arm_z]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_arm_z]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_arm_z]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_arm_z]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_arm_z]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_arm_z]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_arm_z]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_arm_z]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_arm_z]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_arm_z]->dir_x_;
	DrawStringEx(10, 205, -1, "4:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 230, -1, "4:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 245, -1, "4:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --5.e_wrist_z -- //
	//arm_r->modules_[Agn_armR001::e_wrist_z]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_z]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_wrist_z]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_z]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_wrist_z]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_wrist_z]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_wrist_z]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_wrist_z]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_wrist_z]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_wrist_z]->dir_x_;
	DrawStringEx(10, 260, -1, "5:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 275, -1, "5:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 290, -1, "5:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --6.e_wrist_z -- //
	//arm_r->modules_[Agn_armR001::e_wrist_x]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_x]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_wrist_x]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_x]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_wrist_x]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_wrist_x]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_wrist_x]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_wrist_x]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_wrist_x]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_wrist_x]->dir_x_;
	DrawStringEx(10, 305, -1, "6:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 320, -1, "6:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 335, -1, "6:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// --7.e_wrist_z2 -- //
	//arm_r->modules_[Agn_armR001::e_wrist_z2]->cnt_targets_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_z2]->cnt_targets_.push_back(aim_tar_);
	//arm_r->modules_[Agn_armR001::e_wrist_z2]->cnt_objects_.clear();
	//arm_r->modules_[Agn_armR001::e_wrist_z2]->cnt_objects_.push_back(aim_obj_);
	//arm_r->modules_[Agn_armR001::e_wrist_z2]->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
	//tmp_pos = arm_r->modules_[Agn_armR001::e_wrist_z2]->pos_o_next_;
	//tmp_q = arm_r->modules_[Agn_armR001::e_wrist_z2]->rot_tmp_;
	debAxis = arm_r->modules_[Agn_armR001::e_wrist_z2]->rot_axis_;
	debDirz = arm_r->modules_[Agn_armR001::e_wrist_z2]->dir_z_;
	debDirx = arm_r->modules_[Agn_armR001::e_wrist_z2]->dir_x_;
	DrawStringEx(10, 350, -1, "7:axis x=%f, y=%f, z=%f", debAxis.x, debAxis.y, debAxis.z);
	DrawStringEx(10, 365, -1, "7:dirz x=%f, y=%f, z=%f", debDirz.x, debDirz.y, debDirz.z);
	DrawStringEx(10, 380, -1, "7:dirx x=%f, y=%f, z=%f", debDirx.x, debDirx.y, debDirx.z);

	//// 描画アップデート
	//arm_r->update(delta_time);
	// --------------------------------------試作終了 ----------------------------------


	// target 操作 
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		aim_tar_->pos_ += tnl::Vector3{ 0, 10, 0 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		aim_tar_->pos_ += tnl::Vector3{ 0, -10, 0 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT)) {
		aim_tar_->pos_ += tnl::Vector3{ 0, 0, 10 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		aim_tar_->pos_ += tnl::Vector3{ 0, 0, -10 };
	}
	//aim_tar_->Rotate(tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(1)));
	
	
	
	/*DrawStringEx(10, 350, -1, "E.E. pos: x=%f, y=%f, z=%f", effector_obj_->pos_.x, effector_obj_->pos_.y, effector_obj_->pos_.z);
	DrawStringEx(10, 365, -1, "E.E. drz: x=%f, y=%f, z=%f", effector_obj_->dir_z_.x, effector_obj_->dir_z_.y, effector_obj_->dir_z_.z);
	DrawStringEx(10, 380, -1, "E.E. drx: x=%f, y=%f, z=%f", effector_obj_->dir_x_.x, effector_obj_->dir_x_.y, effector_obj_->dir_x_.z);	*/
	aim_tar_->update(0);
	DrawStringEx(10, 395, -1, "T.G. pos: x=%f, y=%f, z=%f", aim_tar_->pos_.x, aim_tar_->pos_.y, aim_tar_->pos_.z);
	DrawStringEx(10, 410, -1, "T.G. drz: x=%f, y=%f, z=%f", aim_tar_->dir_z_.x, aim_tar_->dir_z_.y, aim_tar_->dir_z_.z);
	DrawStringEx(10, 425, -1, "T.G. drx: x=%f, y=%f, z=%f", aim_tar_->dir_x_.x, aim_tar_->dir_x_.y, aim_tar_->dir_x_.z);
	elbow_r_tar->update(0);
	//effector_obj_->update(0);


}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(camera_, 50, 20);

	// ---- 試験 ---- //
	aim_tar_->render(camera_);
	elbow_r_tar->render(camera_);
	effector_obj_->render(camera_);
	elbow_obj_->render(camera_);

	arm_r->render(camera_);
}
