#include "gm_kinematics.h"

void tol::Kinematics::init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child) {
	initDKSetting(parent, child);
}

void tol::Kinematics::update(float delta_time, std::shared_ptr<Object> obj) {

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