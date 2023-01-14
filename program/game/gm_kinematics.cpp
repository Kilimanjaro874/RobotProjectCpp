#include "gm_kinematics.h"
#include "gm_object.h"

void tol::Kinematics::init(const std::shared_ptr<Object> obj) {

}

void tol::Kinematics::update(float delta_time) {

}

void tol::Kinematics::initDKSetting(std::shared_ptr<tol::Object> obj) {
	std::weak_ptr<tol::Object> parent_w = obj->getParent();
	std::shared_ptr<tol::Object> parent_s = parent_w.lock();
	tnl::Vector3 p_pos;
	if (parent_s) {
		p_pos = parent_s->coordinate_.getPos();
	}
	else {
		std::cout << "error [Kinematics] initDKSetting" << std::endl;
	}
	tnl::Vector3 c_pos = obj->coordinate_.getPos();
	// store info. to dk_data_st_
	tnl::Vector3 diff = p_pos - c_pos;
	obj->kinematics_->dk_data_st_.len_p_to_c_ = diff.length();
	diff.normalize();
	obj->kinematics_->dk_data_st_.dir_p_to_c_ = diff;
}