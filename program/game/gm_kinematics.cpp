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
	dk_data_upd_st_ = dk_data_st_;
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
	tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(0.01)) * parent_kinematics->getRotOneFlame();
	kinematics->setRotOneFlame(rot);
	// --- update coordinate --- //
	coordinate->setAddRot(rot);
	kinematics->setRotDkData(coordinate->getRot());
	tnl::Vector3 pos = parent_coordinate->getPos() + 
		kinematics->dk_data_upd_st_.dir_c_p_ * kinematics->dk_data_upd_st_.len_c_p_;
	coordinate->setPos(pos);
}