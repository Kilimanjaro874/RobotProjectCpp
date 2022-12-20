#pragma once
#include "../library/tnl_vector.h"
#include <vector>
#include "../dxlib_ext/dxlib_ext_mesh.h"
#include "../game/model/gm_shot_taget.h"

class HitManager {
public:

	void CheckBulletTargetHit(std::vector<dxe::Mesh*>& bullets, std::vector<ShotTarget*>& targets);
};