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
	GameManager* mgr = GameManager::GetInstance();
	DrawStringEx(50, 50, -1, "Result");
	DrawStringEx(50, 70, -1, "Clear Time : %5.2f sec", mgr->clear_time_);
}
