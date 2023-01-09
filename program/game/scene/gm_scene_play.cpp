#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../../dxlib_ext/dxlib_ext.h"


ScenePlay::~ScenePlay() {
	
}


void ScenePlay::initialzie() {
	
	GameManager* mgr = GameManager::GetInstance();

	_camera = new GmCamera();
	_camera->pos_ = { 0, 150, -300 };

	
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	
	
}

void ScenePlay::render()
{
	_camera->update();
	
	DrawGridGround(_camera, 5, 300);
	
}
