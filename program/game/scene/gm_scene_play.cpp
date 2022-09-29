#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

ScenePlay::~ScenePlay() {
	delete camera_;
	delete elbow_r_tar;
	delete aim_tar_;
	delete arm_r;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 100, 0, -300 };

	// ---- Œ± ----- //
	aim_tar_ = new FaceVec();
	aim_tar_ = FaceVec::Create({ 30, 0, 50});
	aim_tar_->Rotate(tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(-90)));
	aim_tar_->update(0);

	aim_obj_ = new FaceVec();
	aim_obj_ = FaceVec::Create({ 0, 50, 0 });
	//aim_obj_->Rotate(tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90)));
	aim_obj_->update(0);


	arm_r = Agn_armR001::Create(tnl::Vector3{ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	arm_r->update(0);
	arm_r->aimTarget_initialize(*aim_tar_, *aim_obj_, *aim_obj_);

	arm_r->cnt_objects_.push_back(aim_obj_);
	arm_r->cnt_objects_.push_back(aim_tar_);
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

	// ---- Œ± ---- //
	arm_r->update(delta_time);
	arm_r->aimTarget_update(delta_time, *aim_tar_);
	aim_tar_->pos_ += tnl::Vector3{ 0.1, 0.10, -0.10 };

	aim_tar_->update(0);
	aim_obj_->update(0);
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(50, 20);

	// ---- Œ± ---- //
	aim_tar_->render(camera_);
	aim_obj_->render(camera_);
	arm_r->render(camera_);



}
