#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../../dxlib_ext/dxlib_ext.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	
}


void ScenePlay::initialzie() {
	
	GameManager* mgr = GameManager::GetInstance();
	// ---- get instance ---- //
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	assem_repo_ =  tol::AssemRepo::Create();
	actor_ = tol::Actor::Create(assem_repo_, robot_actor_);
	//test_ = assem_repo_->getAssemble(200);
	
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	actor_->updateTree(delta_time);

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
	
}

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 5, 300);

	actor_->renderTree(camera_);
}
