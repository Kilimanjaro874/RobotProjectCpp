#include "gm_circle_collider.h"
#include "gm_manager.h"

void tol::CircleCollider::update(float delta_time) {
	is_hit_ = false;
}

void tol::CircleCollider::hitCheck(std::shared_ptr<Object> this_obj, std::shared_ptr<Object> other_obj){
	if(!other_obj->getCircleCollider()) { return; }
	float other_cir_radius = other_obj->getCircleCollider()->getCircleSize();
	tnl::Vector3 other_pos = other_obj->getCoordinate()->getPos();
	tnl::Vector3 this_pos = this_obj->getCoordinate()->getPos();
	tnl::Vector3 length = other_pos - this_pos;
	if ((radius_ + other_cir_radius) > length.length()) {
		is_hit_ = true;
		GameManager* mgr = GameManager::GetInstance();
		mgr->sound_mgr_->playSound(mgr->sound_mgr_->se, 3, "", mgr->sound_mgr_->one_shot);

		return;
	}
}