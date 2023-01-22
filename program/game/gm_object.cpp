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

/// <summary>
/// Move the object coordinate to the velue of [move].
/// * move_type : 
///		absolute(default) : the object moves to the position of [move].
///		relative : the object moves current position + move amound.
/// </summary>
/// <param name="move"> object move pos or amount of movement.</param>
/// <param name="type"> set move type. </param>
void tol::Object::Transform(tnl::Vector3 move, move_type type) {
	std::shared_ptr<Coordinate> cod = getCoordinate();
	std::shared_ptr<Kinematics> kin = getKinematics();
	if (cod == nullptr || kin == nullptr) { return; }
	if (move_type::absolute == type) {
		// move
		cod->setPos(move);
	}
	else if (move_type::relative == type) {
		// move 
		cod->setPos(cod->getPos() + move);
	}
	// reset dk_setting
	std::weak_ptr<Object> parent_w = getParent();
	std::shared_ptr<Object> parent_s = parent_w.lock();
	if (parent_s) {
		// nonparent : Unnecessary dk_init.
		kin->init(parent_s, shared_from_this());
	}
}

/// <summary>
/// Rotate the object by the value of [rot]
/// * move_type : 
///		absolute(default) : 
/// </summary>
/// <param name="rot"></param>
/// <param name="type"></param>
void tol::Object::Rotation(tnl::Quaternion rot, move_type type) {

}

tnl::Vector3 tol::Object::getRight()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (cod == nullptr) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirX();
}

tnl::Vector3 tol::Object::getUp()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (cod == nullptr) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirY();
}

tnl::Vector3 tol::Object::getForward()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (cod == nullptr) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirZ();
}

void tol::Object::setRenderScale(float size)
{
	std::shared_ptr<Assemble> assem = getAssemble();
	if (assem == nullptr) { return; }
	assem->setPartsScale(size);
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