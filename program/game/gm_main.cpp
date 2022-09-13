#include <time.h>
#include "gm_main.h"
#include "gm_manager.h"

void gameMain(float delta_time) {
	GameManager* mgr = GameManager::GetInstance();
	mgr->update(delta_time);
}

void gameEnd() {
}
