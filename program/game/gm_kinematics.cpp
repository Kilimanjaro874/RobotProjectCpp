#include "gm_kinematics.h"
#include "gm_object.h"
#include "gm_coordinate.h"

void tol::Kinematics::init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child) {
	initDKSetting(parent, child);
}

void tol::Kinematics::update(float delta_time, std::shared_ptr<Object> obj) {
	doKinematics(delta_time, obj);
	if (restraint_) {
		restraint_->update(delta_time, obj);
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
void tol::Kinematics::Translate(std::shared_ptr<Object> obj, tnl::Vector3 move, bool absolute_move) {
	//std::shared_ptr<Coordinate> cod = getCoordinate();
	//std::shared_ptr<Kinematics> kin = getKinematics();
	std::shared_ptr<Coordinate> cod = obj->getCoordinate();
	if (cod == nullptr) { return; }
	if (absolute_move) {
		// move
		cod->setPos(move);
	}
	else {
		// move 
		cod->setPos(cod->getPos() + move);
	}
	// reset dk_setting
	std::weak_ptr<Object> parent_w = obj->getParent();
	std::shared_ptr<Object> parent_s = parent_w.lock();
	if (parent_s) {
		// nonparent : Unnecessary dk_init.
		init(parent_s, obj);
	}
}

/// <summary>
/// Rotate the object by the value of [rot]
/// * bool absolute : 
///		true : set rotation for one frame.(one_flame_rot = rot)
///		false(default) : add rotation for one flame.(one_flame_rot *= rot)
/// </summary>
/// <param name="rot"> rotation for one frame. </param>
/// <param name="type"> set/add to rot.</param>
void tol::Kinematics::Rotation(std::shared_ptr<Object> obj, tnl::Quaternion rot, bool absolute_move) {
	if (absolute_move) {
		setRotOneFlame(rot);
	}
	else {
		setAddRotOneFlame(rot);
	}
}

std::shared_ptr<tol::Kinematics> tol::Kinematics::copyKinematics(std::shared_ptr<Object> parent, std::shared_ptr<Object> child) {
	std::shared_ptr<Kinematics> tmp_kine = std::make_shared<Kinematics>(Kinematics());
	tmp_kine->init(parent, child);
	return tmp_kine;
}

/// <summary>
/// store coordinate difference info.
/// (this - parent coordinate origine)
/// </summary>
/// <param name="obj"> Object class (owner) </param>
void tol::Kinematics::initDKSetting(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child) {
	tnl::Vector3 diff;
	auto parent_coord = parent->getCoordinate();
	auto child_coord = child->getCoordinate();
	diff = child_coord->getPos() - parent_coord->getPos();
	// --- set data --- //
	float length = diff.length();
	diff.normalize();
	dk_data_st_.dir_c_p_ = diff;
	dk_data_st_.len_c_p_ = length;
	dk_data_st_.rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	is_dk_init_ = true;			// set flag
}

void tol::Kinematics::doKinematics(float delta_time, std::shared_ptr<Object> obj) {
	if (!obj->getIsPositionalParentage()) { return; }
	// --- get informations --- //
	std::shared_ptr<Coordinate> coordinate = obj->getCoordinate();	// get coordinate
	std::weak_ptr<Object> parent_w = obj->getParent();		// get parent;
	std::shared_ptr<Object> parent_s = parent_w.lock();
	std::shared_ptr<Kinematics> kinematics = obj->getKinematics();
	std::shared_ptr<Kinematics> parent_kinematics;
	std::shared_ptr<Coordinate> parent_coordinate;
	// --- null check --- //
	if (parent_s) {
		// -- Child Object task -- //
		parent_kinematics = parent_s->getKinematics();
		parent_coordinate = parent_s->getCoordinate();
	}
	else {
		// -- Root Object task -- //
		coordinate->setAddRot(kinematics->getRotOneFlame());
		return;		// do not kinematics.
	}
	// --- get rot while one flame --- //
	// -- reflact parent's rot influence or not. -- //
	tnl::Quaternion rot_one_flame = parent_kinematics->getRotOneFlame();	// Reflect parent's rotate info.
	// --- update coordinate --- //
	kinematics->setRotOneFlame(rot_one_flame);
	kinematics->setRotDkData(rot_one_flame);
	// -- pos -- //
	// - reflect parent's pos influence or not - //
	tnl::Vector3 pos = parent_coordinate->getPos() + kinematics->dk_pos_parent_to_this_;
	coordinate->setPos(pos);
	// -- rot -- //
	coordinate->setAddRot(rot_one_flame);
	// --- do Inverse Kinematics --- //
	tnl::Quaternion rot_ik_one_flame = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	if (inv_kinematics_ != nullptr) {
		rot_ik_one_flame = inv_kinematics_->update(delta_time, obj);
	}
	// --- reupdate coordinate --- //
	kinematics->setAddRotOneFlame(rot_ik_one_flame);
	coordinate->setAddRot(rot_ik_one_flame);
}

