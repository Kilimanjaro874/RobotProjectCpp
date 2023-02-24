#include "gm_assemble.h"
#include "gm_object.h"
#include "gm_coordinate.h"

// Assemble update(pos, rot) : Used by Object Class. (default)
void tol::Assemble::update(float delta_time, std::shared_ptr<tol::Object> obj) {
	 auto cod = obj->getCoordinate();
	 tnl::Quaternion rot = ofs_rot_ *  cod->getRot();
	 //tnl::Quaternion rot = cod->getRot();
	 tnl::Vector3 pos = cod->getPos() + tnl::Vector3::TransformCoord(ofs_pos_, rot);
	 //tnl::Vector3 pos = cod->getPos();
	 if (is_render_) {
		 for (auto p : parts_) {
			 p->mesh_->pos_ = pos + tnl::Vector3::TransformCoord(p->ofs_pos_, rot);
			 p->mesh_->rot_q_ = p->ofs_rot_ * rot;
		 }
	 }
	 rot = cod->getRot();
	 pos = cod->getPos();
	 if (is_coordinate_render_) {
		 for (auto p : coordinate_) {
			 p->mesh_->pos_ = pos + tnl::Vector3::TransformCoord(p->ofs_pos_, rot);
			 p->mesh_->rot_q_ = p->ofs_rot_ * rot;
		 }
	 }
}

void tol::Assemble::render(dxe::Camera* camera) {
	if (is_render_) {
		for (auto p : parts_) {
			p->mesh_->render(camera);
		}
	}
	if (is_coordinate_render_) {
		for (auto p : coordinate_) {
			p->mesh_->render(camera);
		}
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

/// <summary>
/// If you want to visualize the coordinates: call this func.
/// </summary>
/// <param name="length"> visualize coordinate length x, y, z </param>
/// <param name="radius"> the cylinder thickness </param>
void tol::Assemble::setCoordinateView(std::shared_ptr<Object> obj, float length, float radius) {
	auto cod = obj->getCoordinate();	// get coordinate.
	is_coordinate_render_ = true;
	std::shared_ptr<Parts> x = std::make_shared<Parts>(Parts());	// x
	x->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	x->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	x->ofs_pos_ = cod->getDirX() * length / 2.0;
	x->ofs_rot_ = tnl::Quaternion::RotationAxis(cod->getDirZ(), tnl::ToRadian(90)) * cod->getRot();
	coordinate_.push_back(x);
	std::shared_ptr<Parts> y = std::make_shared<Parts>(Parts());	// y
	y->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	y->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	y->ofs_pos_ = cod->getDirY() * length / 2.0;
	y->ofs_rot_ = tnl::Quaternion::RotationAxis(cod->getDirZ(), tnl::ToRadian(0)) * cod->getRot();
	coordinate_.push_back(y);
	std::shared_ptr<Parts> z = std::make_shared<Parts>(Parts());	// z
	z->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	z->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	z->ofs_pos_ = cod->getDirZ() * length / 2.0;
	z->ofs_rot_ = tnl::Quaternion::RotationAxis(cod->getDirX(), tnl::ToRadian(90)) * cod->getRot();
	coordinate_.push_back(z);
}

/// <summary>
/// Return a copy of this Assemble Class
/// </summary>
/// <returns> Copied this class </returns>
std::shared_ptr<tol::Assemble> tol::Assemble::copyAssemble() {
	std::shared_ptr<Assemble> tmp_assem = std::make_unique<Assemble>(Assemble());
	std::vector<std::shared_ptr<Parts>> parts = CopyParts();
	for (int i = 0; i < parts.size(); i++) {
		tmp_assem->setParts(parts[i]);
	}
	return tmp_assem;
}