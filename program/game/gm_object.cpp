#include "gm_object.h"


void tol::Object::update(float delta_time) {
	coordinate_.update(delta_time);
	assemble_->update(delta_time, coordinate_.getPos(), coordinate_.getRot());
}

void tol::Object::render(dxe::Camera* camera) {
	assemble_->render(camera);
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


std::shared_ptr<tol::Object> tol::Object::getParentTree(int& id, std::string& name) {
	if(id_ == id || name_ == name){
		return shared_from_this();
	}
	for (auto itr = children_.begin(); itr != children_.end(); itr++) {
		auto child = *itr;
		std::shared_ptr<tol::Object> obj = child->getParentTree(id, name);
		if (obj == nullptr) {
			continue; 
		}
		if (obj->id_ == id || obj->name_ == name) { 
			return obj; 
		}
	}
	return nullptr;
}
