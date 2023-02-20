#include "gm_weapon.h"

tol::Weapon::Weapon(
	std::shared_ptr<Object> parent,
	std::shared_ptr<Assemble> bullet_assem,
	float reload_time, float bullet_damage,
	float bullet_speed, float bullet_size
	)
{
	parent_ = parent;
	bullet_assem_ = bullet_assem;
	reload_time_ = reload_time;
	bullet_damage_ = bullet_damage;
	bullet_speed_ = bullet_speed;
	bullet_size_ = bullet_size;
}

void tol::Weapon::update(float delta_time) {
	reload_count_ += delta_time;
	if (reload_count_ > reload_time_ && is_fire_permit_) {
		// --- bullet generation --- //
		genBullet();
	}

	is_fire_permit_ = false;
}

void tol::Weapon::genBullet() {
	auto parent_obj = parent_.lock();
	if (!parent_obj) {
		printf("error : parent object not registered.\n");
		return;
	}
	auto parent_cod = parent_obj->getCoordinate();
	// --- gen bullet --- //
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(Bullet(
		parent_cod->getPos(),
		parent_cod->getDirZ(),
		bullet_damage_,
		bullet_speed_
	));
	// --- registored bullet to list --- //
	bullets_.push_back(bullet);
}

////////////////////////////////
//// ---- Bullet class ---- ////
tol::Weapon::Bullet::Bullet(
	tnl::Vector3 pos,
	tnl::Vector3 dir,
	float bullet_damage,
	float bullet_speed
) {
	pos_ = pos;
	forward_dir_ = dir;
	bullet_damage_ = bullet_damage;
	bullet_speed_ = bullet_speed;
}

void tol::Weapon::Bullet::update(float delta_time) {

}

void tol::Weapon::Bullet::render(dxe::Camera* camera) {

}