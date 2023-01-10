#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../../dxlib_ext/dxlib_ext.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	delete assem_repo_;
	delete test_;
}


void ScenePlay::initialzie() {
	
	GameManager* mgr = GameManager::GetInstance();
	// ---- get instance ---- //
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	assem_repo_ = tol::AssemRepo::Create();
	test_ = assem_repo_->getAssemble(200);
	
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	
	
}

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 5, 300);
	test_->render(camera_);
	
}
