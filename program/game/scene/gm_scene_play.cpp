#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../model//gm_airplane.h"
#include "../model//gm_robot_module.h"
#include "../model/gm_robot_agent.h"

#include "../model/gm_ag_arm_r.h";

#include "../model/gm_player.h"

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

	// -- 試験：飛行機を出す -- //
	//plane_ = MdlAirPlane::Create();
	// -- 自作ロボットモジュールを出す -- //
	//arm01_ = MdlRobotModule::Create();
	/*armAgnt01_ = MdlRobotAgent_Rarms::Create();
	armAgnt01_->rot_ = tnl::Quaternion::RotationAxis(tnl::Vector3{ 0, 0, 1 }, 0);
	armAgnt01_->directKinematics();
	armAgnt01_->directKinematics();*/

	


	arm_r_ = MdlArm_r::Create();
	
	tnl::Vector3 tmp_pos = tnl::Vector3{ 0, 0, 0 };
	tnl::Quaternion tmp_q_back = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	tnl::Vector3 tmp_l_back = { 0, 0, 0 };
	arm_r_->calcLDK(tmp_pos, tmp_q_back, tmp_l_back);
	arm_r_->update(0);
	arm_r_->render(camera_);


//	player_ = Player::Create();
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

	// -- 試験：飛行機の計算 -- //
	//plane_->update(delta_time);
	// -- 自作ロボットモジュールの計算
	//arm01_->update(delta_time);
	//armAgnt01_->directKinematics();
	//armAgnt01_->update(delta_time);
	//armAgnt01_->directKinematics();


	/*armAgnt01_->directKinematics();

	tnl::Vector3 euler0 = armAgnt01_->modules_[0]->rot_.getEuler();
	DrawStringEx(50, 30, -1, "P0.x = %f, P0.y = %f, rotz = %f", 
		armAgnt01_->modules_[0]->pos_.x, armAgnt01_->modules_[0]->pos_.y,
		tnl::ToDegree(euler0.z));
	
	tnl::Vector3 euler1 = armAgnt01_->modules_[1]->rot_.getEuler();
	DrawStringEx(50, 50, -1, "P1.x = %f, P1.y = %f, rotz = %f",
		armAgnt01_->modules_[1]->pos_.x, armAgnt01_->modules_[1]->pos_.y,
		tnl::ToDegree(euler1.z));

	tnl::Vector3 euler2 = armAgnt01_->modules_[2]->rot_.getEuler();
	DrawStringEx(50, 70, -1, "P2.x = %f, P2.y = %f, rotz = %f",
		armAgnt01_->modules_[2]->pos_.x, armAgnt01_->modules_[2]->pos_.y,
		tnl::ToDegree(euler2.z));
	
	DrawStringEx(50, 90, -1, "L0.x = %f, L0.y = %f",
		armAgnt01_->modules_[0]->parts_[0]->ofs_pos_.x,
		armAgnt01_->modules_[0]->parts_[0]->ofs_pos_.y);*/

	

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
	tnl::Vector3 tmp_l_back = { 0, 0, 0 };
	arm_r_->calcLDK(tmp_pos, tmp_q_back, tmp_l_back);
	arm_r_->update(delta_time);


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
