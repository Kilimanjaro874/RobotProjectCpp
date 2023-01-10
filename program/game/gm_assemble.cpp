#include "gm_assemble.h"

void tol::Assemble::update(float delta_time) {
	if (!is_update_) { return; }
	for (auto m : mesh_v_) {
		m->pos_ = offset_pos_ + tnl::Vector3::TransformCoord(m->pos_, rot_);
		m->rot_q_ = m->rot_q_ * rot_;
	}
}

void tol::Assemble::render(dxe::Camera* camera) {
	if (!is_render_) { return; }
	for (auto m : mesh_v_) {
		m->render(camera);
	}
}

void tol::Assemble::setScale(const tnl::Vector3& scale) {
	for (auto m : mesh_v_) {
		m->scl_ = scale;
	}
}