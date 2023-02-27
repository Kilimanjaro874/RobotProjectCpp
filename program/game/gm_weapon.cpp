#include "gm_weapon.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_manager.h"

void tol::Weapon::update(float delta_time, std::shared_ptr<Object> obj)
{
	reload_count_ += delta_time;
	is_fired_ = false;
	// --- fire task : generate bullet --- //
	if (is_fire_ && reload_count_ >= reload_time_) {
		reload_count_ = 0.0;
		genBullet(obj);
		GameManager* mgr = GameManager::GetInstance();
		is_fired_ = true;
	}
	is_fire_ = false;
	// --- bullets update --- //
	for (auto itr = bullets_.begin(); itr != bullets_.end();) {
		auto tmp_bullet = *itr;
		auto bullet_kin = tmp_bullet->getKinematics();
		// --- set state & flag check --- //
		float flight_distance = tmp_bullet->getOptionParams(0);
		flight_distance += bullet_speed_ * delta_time;
		if (flight_distance > bullet_effective_range_) {
			tmp_bullet->setIsAlive(false);
		}
		if (!tmp_bullet->getIsAlive()) {
			itr = bullets_.erase(itr);
			continue;
		}
		// --- move update --- //
		tmp_bullet->setOptionParams(0, flight_distance);
		if (bullet_dir::front == bullet_dir_) {

			auto cod = tmp_bullet->getCoordinate();
			bullet_kin->Translate(
				tmp_bullet,
				tmp_bullet->getCoordinate()->getDirZ() * bullet_speed_ * delta_time,
				false
				);
		}
		if (bullet_dir::right == bullet_dir_) {
			tmp_bullet->getKinematics()->Translate(
				tmp_bullet,
				tmp_bullet->getCoordinate()->getDirX() * bullet_speed_ * delta_time,
				false
			);
		}
		if (bullet_dir::up == bullet_dir_) {
			tmp_bullet->getKinematics()->Translate(
				tmp_bullet,
				tmp_bullet->getCoordinate()->getDirY() * bullet_speed_ * delta_time,
				false
			);
		}
		tmp_bullet->update(delta_time);
		
		itr++;
	}
}

void tol::Weapon::render(dxe::Camera* camera) {
	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		auto tmp_bullet = *itr;
		tmp_bullet->render(camera);
	}
}

void tol::Weapon::genBullet(std::shared_ptr<Object> obj) {
	// --- gen bullet : copy parent cod, bullet assemble.
	std::shared_ptr<Object> tmpbullet = std::make_shared<Object>(Object(-1, "bullet"));
	auto cod = obj->getCoordinate();
	auto bullet_cod = cod->copyCoordinate();
	auto assem = bullet_->getAssemble();
	auto bullet_assem = assem->copyAssemble();
	auto bullet_kin = obj->getKinematics();
	tmpbullet->init(
		bullet_cod,
		bullet_assem,
		bullet_kin
	);

	tmpbullet->setIsPositionalParentage(false);		// ignore parend DK.
	tmpbullet->setOptionParams(1, bullet_damage_);	// set bullet damage to Object option num.
	bullets_.push_back(tmpbullet);					// registor to list.
}