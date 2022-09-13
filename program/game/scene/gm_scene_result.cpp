#include "../gm_manager.h"
#include "gm_scene_result.h"
#include "gm_scene_title.h"

void SceneResult::initialzie() {
}

void SceneResult::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();


	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneTitle());
	}
}

void SceneResult::render()
{
	DrawStringEx(50, 50, -1, "scene result");
}
