#include "gm_weapon.h"

void tol::Weapon::update(float delta_time, std::shared_ptr<Object> obj)
{
	reload_count_ += delta_time;
	// --- fire task : generate bullet --- //
	if (is_fire_ && reload_count_ >= reload_time_) {
		reload_count_ = 0;
		is_fire_ = false;
		genBullet(obj);
	}
	// --- bullets update --- //
	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		auto tmp_bullet = *itr;
		auto bullet_kin = tmp_bullet->getKinematics();
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

	/*tmpbullet->init(
		obj->getCoordinate()->copyCoordinate(),
		bullet_->getAssemble()->copyAssemble(),
		obj->getKinematics()->copyKinematics(obj, tmpbullet)
		);*/
	tmpbullet->init(
		bullet_cod,
		bullet_assem,
		bullet_kin
	);

	tmpbullet->setIsPositionalParentage(false);	// ignore parend DK.
	bullets_.push_back(tmpbullet);				// registor to list.
}