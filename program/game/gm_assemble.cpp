#include "gm_assemble.h"

void tol::Assemble::update(float delta_time, std::shared_ptr<Object> obj) {

}

void tol::Assemble::render(dxe::Camera* camera) {
	if (!is_render) { return; }
	for (auto p : parts_) {
		p->mesh_->render(camera);
	}
}

std::vector<std::shared_ptr<Parts>> tol::Assemble::CopyParts() {
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

void tol::Assemble::setPartsScale(float scale) {
	for (auto p : parts_) {
		p->mesh_->scl_ = scale;
		p->ofs_pos_ *= scale;
	}
}