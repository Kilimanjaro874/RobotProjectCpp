#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	
	delete armR_;
	for (auto tar : targets_) { delete tar; }
	for (auto obj : objects_) {	delete obj;	}
}


void ScenePlay::initialzie() {

	camera_ = new GmCamera();
	armR_ = Agn_armR001::Create(tnl::Vector3{ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));

	targets_.resize(2);
	targets_[0] = FaceVec::Create({ 0, 0, 70 });
	targets_[1] = FaceVec::Create({ 30, -30, 0 });
	
	objects_.resize(2);
	objects_[0] = FaceVec::Create({ 0, 0, 0 });
	objects_[1] = FaceVec::Create({ 0, 0, 0 });

	armR_->mode01_init(targets_);

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
	// --- ƒV[ƒ“Ø‚è‘Ö‚¦ --- //
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	for (auto tar : targets_) {
		tar->update(delta_time);
	}


	// Œ±“®ì
	armR_->mode01_update(delta_time);

	// target ‘€ì 
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		targets_[0]->pos_ += tnl::Vector3{ 0, 10, 0 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		targets_[0]->pos_ += tnl::Vector3{ 0, -10, 0 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT)) {
		targets_[0]->pos_ += tnl::Vector3{ 0, 0, 10 };
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
		targets_[0]->pos_ += tnl::Vector3{ 0, 0, -10 };
	}
	
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(camera_, 50, 20);

	armR_->render(camera_);

	for (int i = 0; i < 2; i++) {
		targets_[i]->render(camera_);
		armR_->cnt_objects_[i]->render(camera_);
	}
	
}
