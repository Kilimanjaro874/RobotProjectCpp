#include "gm_assemble.h"

void tol::Assemble::update(float delta_time, const tnl::Vector3& coordinate_pos, const tnl::Quaternion& coordinate_rot) {
	if (!is_update_) { return; }
	rot_ = rot_ * coordinate_rot;
	for (auto p : parts_) {
		p->mesh_->pos_ = coordinate_pos + 
			tnl::Vector3::TransformCoord(offset_pos_, rot_) + 
			tnl::Vector3::TransformCoord(p->ofs_pos_, rot_);
		p->mesh_->rot_q_ = p->ofs_rot_ * rot_;
	}
}

void tol::Assemble::render(dxe::Camera* camera) {
	if (!is_render_) { return; }
	for (auto p : parts_) {
		p->mesh_->render(camera);
	}
}

void tol::Assemble::setScale(float scale) {
	for (auto p : parts_) {
		p->mesh_->scl_ = scale;
		p->ofs_pos_ *= scale;
	}
}

std::vector<std::shared_ptr<Parts>> tol::Assemble::CreateCopyParts() {
	std::vector<std::shared_ptr<Parts>> parts;
	parts.resize(parts_.size());
	for (int i = 0; i < parts.size(); i++) {
		parts[i] = std::make_shared<Parts>(Parts());
		parts[i]->ofs_pos_ = parts_[i]->ofs_pos_;
		parts[i]->ofs_rot_ = parts_[i]->ofs_rot_;
		parts[i]->mesh_ = parts_[i]->mesh_->createClone();
	}
	return parts;
}