#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../model//gm_airplane.h"
#include "../model//gm_robot_module.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	//delete plane_;
	delete arm01_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 0, -200 };

	// -- 試験：飛行機を出す -- //
	//plane_ = MdlAirPlane::Create();
	// -- 自作ロボットモジュールを出す -- //
	arm01_ = MdlRobotModule::Create();

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
	arm01_->update(delta_time);
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(50, 20);

	//DrawOBB(plane_->pos_, plane_->rot_, { 32, 48, 32 });

	// -- 試験：飛行機の描画 -- //
	//plane_->render(camera_);
	arm01_->render(camera_);
}
