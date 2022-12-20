#include "gm_hit_manager.h"

void HitManager::CheckBulletTargetHit(std::vector<Weapon::Bullet*> bullets, std::vector<ShotTarget*> targets,  BGM_SE_Mgr* bs_mgr) {
	for (auto b : bullets) {
		for (auto t : targets) {
			tnl::Vector3 tmp = b->_pos - t->_pos;
			if (tmp.length() <(t->_circle_size + b->_circle_size)) {
				bs_mgr->playSound(bs_mgr->se, 0, "TargetDamaged.mp3", bs_mgr->one_shot);
				t->hit(b->_damage);
			}
		}
	}
}