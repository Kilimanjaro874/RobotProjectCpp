#include "gm_kinematics.h"

void tol::Kinematics::init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child) {
	initDKSetting(parent, child);
}

void tol::Kinematics::update(float delta_time, std::shared_ptr<Object> obj) {
	doKinematics(delta_time, obj);
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
		// ¦ this task must do in kinematics class 
		tnl::Quaternion rot_one_flame = kinematics->getRotOneFlame();
		coordinate->setAddRot(rot_one_flame);
		return;		// do not kinematics.
	}
	// --- get rot while one flame --- //
	tnl::Quaternion rot_one_flame = parent_kinematics->getRotOneFlame();
	// --- update coordinate --- //
	// --  pos -- //
	kinematics->setRotOneFlame(rot_one_flame);
	kinematics->setRotDkData(rot_one_flame);
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