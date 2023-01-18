#include "gm_assemble.h"
#include "gm_object.h"
#include "gm_coordinate.h"

void tol::Assemble::update(float delta_time, std::shared_ptr<tol::Object> obj) {
	 auto cod = obj->getCoordinate();
	 tnl::Quaternion rot = cod->getRot() * ofs_rot_;
	 tnl::Vector3 pos = cod->getPos() + tnl::Vector3::TransformCoord(ofs_pos_, rot);
	 for (auto p : parts_) {
		 p->mesh_->pos_ = pos + tnl::Vector3::TransformCoord(p->ofs_pos_, rot * p->ofs_rot_);
	 }
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