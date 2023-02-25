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
	coordinate_->init();
	assemble_ = std::make_unique<Assemble>(Assemble());
	kinematics_ = std::make_unique<Kinematics>(Kinematics());
}

void tol::Object::update(float delta_time) {
	if (kinematics_) {
		kinematics_->update(delta_time, shared_from_this());
	}
	if (assemble_) {
		assemble_->update(delta_time, shared_from_this());
	}
	if (weapon_) {
		weapon_->update(delta_time, shared_from_this());
	}
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
	if (weapon_) {
		weapon_->render(camera);
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

/// <summary>
/// Move the object coordinate to the velue of [move].
///  * bool absolute : 
///		true  : the object moves to the position of [move].
///		false(default) : the object moves current position + move amound.(relative)
/// </summary>
/// <param name="move"> object move pos or amount of movement.</param>
/// <param name="type"> set move type. </param>
void tol::Object::Translate(tnl::Vector3 move, bool absolute_move) {
	std::shared_ptr<Kinematics> kin = getKinematics();
	if (kin) {
		kin->Translate(shared_from_this(), move, absolute_move);
	}
}

/// <summary>
/// Rotate the object by the value of [rot]
/// * bool absolute : 
///		true(default) : set rotation for one frame.(one_flame_rot = rot)
///		false : add rotation for one flame.(one_flame_rot *= rot)
/// </summary>
/// <param name="rot"> rotation for one frame. </param>
/// <param name="type"> set/add to rot.</param>
void tol::Object::Rotation(tnl::Quaternion rot, bool absolute_move) {
	std::shared_ptr<Kinematics> kin = getKinematics();
	kin->Rotation(shared_from_this(), rot, absolute_move);
}

tnl::Vector3 tol::Object::getRight()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (!cod) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirX();
}

tnl::Vector3 tol::Object::getUp()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (!cod) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirY();
}

tnl::Vector3 tol::Object::getForward()
{
	std::shared_ptr<Coordinate> cod = getCoordinate();
	if (!cod) { return tnl::Vector3{ 0, 0, 0 }; }
	return cod->getDirZ();
}

void tol::Object::setRenderScale(float size)
{
	std::shared_ptr<Assemble> assem = getAssemble();
	if (!assem) { return; }
	assem->setPartsScale(size);
}

std::shared_ptr<tol::Object> tol::Object::getObjectTree(const int id, const std::string& name) {
	if (id_ == id || name_ == name) {
		return shared_from_this();
	}
	for (auto itr = children_.begin(); itr != children_.end(); itr++) {
		auto child = *itr;
		std::shared_ptr<tol::Object> obj = child->getObjectTree(id, name);
		if (!obj) {
			continue;
		}
		if (obj->id_ == id || obj->name_ == name) {
			return obj;
		}
	}
	return nullptr;
}