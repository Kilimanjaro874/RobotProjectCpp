#include "gm_object.h"

void Object::init(){}

void Object::update(float delta_time) {
	Coordinate::update(delta_time);
	for (auto pts : parts_) {
		pts->mesh_->pos_ = getPos() + tnl::Vector3::TransformCoord(pts->ofs_pos_, getRot());
		pts->mesh_->rot_q_ = pts->ofs_rot_ * getRot();
	}
}

void Object::render(dxe::Camera * camera) {
	Coordinate::render(camera);
	if (!_is_render) { return; }
	for (auto pts : parts_) {
		if (!pts->is_render_) {	continue; }
		pts->mesh_->render(camera);
	}
}

