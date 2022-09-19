#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../model//gm_airplane.h"
#include "../model//gm_robot_module.h"
#include "../model/gm_robot_agent.h"

#include "../model/gm_ag_arm_r.h";


tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	//delete plane_;
	delete arm01_;
	delete armAgnt01_;

	delete arm_r_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 0, -200 };

	arm_r_ = MdlArm_r::Create(targetPos_);
	

	//tnl::Vector3 tmp_pos = tnl::Vector3{ 0, 0, 0 };
	//tnl::Quaternion tmp_q_back = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	//tnl::Vector3 tmp_l_back = { 0, 0, 0 };
	//arm_r_->calcLDK(tmp_pos, tmp_q_back, tmp_l_back);
	arm_r_->update(0);
	arm_r_->render(camera_);


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
		pos_ += forward_ * 10;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		pos_ -= forward_ * 10;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		tempQ_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);

	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		tempQ_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(-3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);
	}
	else {
		tempQ_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(0));
	}



	//arm_r_->testmove();
	tnl::Vector3 tmp_pos = pos_;
	tnl::Quaternion tmp_q_back = tempQ_;

	arm_r_->calcLDKwithLIK(delta_time, tmp_pos, tmp_q_back, arm_r_->target_es, arm_r_->target_pos_);
	arm_r_->update(delta_time);

	for (int i = 0; i < arm_r_->modules_.size(); i++) {
		arm_r_->pos_es_[i] = arm_r_->target_es;
	}

	float p1length = (arm_r_->modules_[1]->pos_ - arm_r_->modules_[0]->pos_).length();
	DrawStringEx(50, 30, -1, "length1 = %f", p1length);
	float p2length = (arm_r_->modules_[2]->pos_ - arm_r_->modules_[1]->pos_).length();
	DrawStringEx(50, 50, -1, "length2 = %f", p2length);



	

	/*player_->move(delta_time);
	tnl::Quaternion tmp_q_back = player_->rot_;
	tnl::Vector3 tmp_l_back = player_->pos_;
	player_->calcLDK();
	player_->update(delta_time);*/
	
	
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(50, 20);

	//DrawOBB(plane_->pos_, plane_->rot_, { 32, 48, 32 });

	// -- 試験：飛行機の描画 -- //
	//plane_->render(camera_);
	//arm01_->render(camera_);
	/*armAgnt01_->render(camera_);*/

	arm_r_->render(camera_);
	//player_->render(camera_);
}
