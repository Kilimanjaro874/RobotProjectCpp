#include "gm_kinematics.h"

void tol::Kinematics::init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child) {
	initDKSetting(parent, child);
}

void tol::Kinematics::update(float delta_time, std::shared_ptr<Object> obj) {
	directKinematics(delta_time, obj);
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
}

void tol::Kinematics::directKinematics(float delta_time, std::shared_ptr<Object> obj) {
	// --- get informations --- //
	std::shared_ptr<Coordinate> coordinate = obj->getCoordinate();	// get coordinate
	std::weak_ptr<Object> parent_w = obj->getParent();		// get parent;
	std::shared_ptr<Object> parent_s = parent_w.lock();
	std::shared_ptr<Kinematics> kinematics = obj->getKinematics();
	std::shared_ptr<Kinematics> parent_kinematics;
	std::shared_ptr<Coordinate> parent_coordinate;
	if (parent_s) {
		parent_kinematics = parent_s->getKinematics();
		parent_coordinate = parent_s->getCoordinate();
	}
	else {
		return;		// do not kinematics.
	}
	// test rotate
	tnl::Quaternion rot_one_flame = parent_kinematics->getRotOneFlame();
	// --- update coordinate --- //
	// --  pos -- //
	kinematics->setRotOneFlame(rot_one_flame);
	kinematics->setRotDkData(rot_one_flame);
	tnl::Vector3 pos = parent_coordinate->getPos() + kinematics->dk_pos_parent_to_this_;
	coordinate->setPos(pos);
	// -- rot -- //
	coordinate->setAddRot(rot_one_flame);
	// --- ik --- //
	tnl::Quaternion rot_ik_one_flame = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(15));
	// --- reupdate coordinate --- //
	kinematics->setAddRotOneFlame(rot_ik_one_flame);
	coordinate->setAddRot(rot_ik_one_flame);
}