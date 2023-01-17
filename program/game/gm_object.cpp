#include "gm_object.h"

void tol::Object::init(
	// store class with information
	std::shared_ptr<Coordinate> cod,
	std::shared_ptr<Assemble> assem,
	std::shared_ptr<Kinematics> kin
	) {
	setCoordinate(cod);
	setAssemble(assem);
	setKinematics(kin);
}

void tol::Object::init() {
	// create empty classes
	coordinate_ = std::make_unique<Coordinate>(Coordinate());
	assemble_ = std::make_unique<Assemble>(Assemble());
	kinematics_ = std::make_unique<Kinematics>(Kinematics());
}

void tol::Object::update(float delta_time) {
	kinematics_->update(delta_time, shared_from_this());
	assemble_->update(delta_time, shared_from_this());
}

void tol::Object::updateTree(float delta_time) {
	update(delta_time);
	if (children_.size() == 0) {
		return;
	}
	for (auto itr = children_.begin(); itr != children_.end(); itr++) {
		auto child = *itr;
		child->updateTree(delta_time);
	}
}

void tol::Object::render(dxe::Camera* camera) {
	assemble_->render(camera);
}

void tol::Object::renderTree(dxe::Camera* camera) {
	render(camera);
	if (children_.size() == 0) {
		return;
	}
	for (auto itr = children_.begin(); itr != children_.end(); itr++) {
		auto child = *itr;
		child->renderTree(camera);
	}
}

std::shared_ptr<tol::Object> tol::Object::getObjectTree(const int id, const std::string& name) {
	if (id_ == id || name_ == name) {
		return shared_from_this();
	}
	for (auto itr = children_.begin(); itr != children_.end(); itr++) {
		auto child = *itr;
		std::shared_ptr<tol::Object> obj = child->getObjectTree(id, name);
		if (obj == nullptr) {
			continue;
		}
		if (obj->id_ == id || obj->name_ == name) {
			return obj;
		}
	}
	return nullptr;
}