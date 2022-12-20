#include "../gm_manager.h"
#include "gm_scene_title.h"
#include "gm_scene_play.h"


void SceneTitle::initialzie() {
	_title_img_hd = LoadGraph("graphics/title.PNG");
}

void SceneTitle::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	if (!_init) {
		_init = true;
		mgr->_soundMgr->playSound(mgr->_soundMgr->bgm, 3, "", mgr->_soundMgr->loop);
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene( new ScenePlay() );
		mgr->_soundMgr->stopSoundAll();
	}
}

void SceneTitle::render()
{
	//DrawStringEx(50, 50, -1, "scene title");
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 0.8, 0, _title_img_hd, true, false);
}
