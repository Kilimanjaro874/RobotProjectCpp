#pragma once
#include "../library/tnl_vector.h"
#include <vector>
#include "../dxlib_ext/dxlib_ext_mesh.h"
#include "../game/model/gm_shot_taget.h"
#include "gm_bgm_se_manager.h"
#include "../game/model/gm_weapon.h"

class HitManager {
public:

	void CheckBulletTargetHit(std::vector<Weapon::Bullet*> bullets, std::vector<ShotTarget*> targets, BGM_SE_Mgr* bs_mgr);
};