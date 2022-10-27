#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	delete target_;
	delete robo_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };

	//------------------------------------------------------------------
	//
	// Test
	//
	target_ = FaceVector::Create(0, tnl::Vector3{ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	robo_ = Robot::Create({ 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	robo_->mode01_init(0);

}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	//------------------------------------------------------------------
	//
	// ƒJƒƒ‰§Œä
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

	//------------------------------------------------------------------
	//
	// Test
	//
	target_->update(delta_time);

	//robo_->updateTree(robo_, delta_time);
	//robo_->mode01_update
	robo_->mode01_update(delta_time);

}

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 1, 200);

	//------------------------------------------------------------------
	//
	// Test
	//
	target_->render(camera_);
	robo_->renderTree(robo_, camera_);
	robo_->targets_[robo_->e_r_arm][0]->render(camera_);

}
