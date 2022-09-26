#include "gm_module.h"

void Module::update(float delta_time) {
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_o_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum;
	}
}

void Module::render(dxe::Camera* camera) {
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}