#include <math.h>
#include "gm_kinematics.h"
#include "gm_object.h"

void tol::Kinematics::init(const std::shared_ptr<Object> obj) {
	initDKSetting(obj);
}

void tol::Kinematics::update(float delta_time, std::shared_ptr<Object> obj) {
	directKinematics(delta_time, obj);
}

void tol::Kinematics::initDKSetting(std::shared_ptr<tol::Object> obj) {
	std::weak_ptr<Object> parent_w = obj->getParent();
	std::shared_ptr<Object> parent_s = parent_w.lock();
	tnl::Vector3 p_pos;
	if (parent_s) {
		p_pos = parent_s->coordinate_.getPos();
	}
	else {
		std::cout << "error [Kinematics] initDKSetting" << std::endl;
	}
	tnl::Vector3 c_pos = obj->coordinate_.getPos();
	// store info. to dk_data_st_
	tnl::Vector3 diff = c_pos - p_pos;
	obj->kinematics_->dk_data_st_.len_p_to_c_ = diff.length();
	diff.normalize();
	obj->kinematics_->dk_data_st_.dir_p_to_c_ = diff;
}

void tol::Kinematics::directKinematics(float delta_time, std::shared_ptr<Object> obj) {
	// ---- update this coordinate ---- //
	setAddRotToRFW(tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(0.001)));
	updateCoordinate(obj);
	
	
	// ---- set rot info to children ---- //
	auto children = obj->getChildren();
	for (auto itr = children.begin(); itr != children.end(); itr++) {
		auto child = *itr;
		child->kinematics_->setRotFromWorld(obj->kinematics_->getRotFromWorld());
	}
}

/// <summary>
/// update Coordinate from root to children.
/// </summary>
/// <param name="rot"> Integrated value of rtation (based on wordl coordinates)</param>
/// <param name="obj"> self coordinate </param>
void tol::Kinematics::updateCoordinate(std::shared_ptr<Object> obj) {
	// ---- update coordinate difference info.(parent - this(child))
	
	// ---- update self coordinate ---- //
	// --- get info. --- //
	std::weak_ptr<Object> parent_w = obj->getParent();
	std::shared_ptr<Object> parent_s = parent_w.lock();
	tnl::Vector3 parent_pos;
	//tnl::Quaternion rot_tmp;
	if (parent_s) {
		parent_pos = parent_s->coordinate_.getPos();
	}
	else {
		// nom parent : root coordinate.
		parent_pos = obj->coordinate_.getPos();
	}
	// --- input info. --- //
	obj->coordinate_.setAddRot(getRotFromWorld());	// rotate coordinate.
	setCoordinateDiffInfo(getRotFromWorld());
	tnl::Vector3 delta_pos_vec = dk_data_st_.dir_p_to_c_ * dk_data_st_.len_p_to_c_;
	tnl::Vector3 pos = parent_pos + delta_pos_vec;
	obj->coordinate_.setPos(pos);			// update coordinate position.

	
}

void tol::Kinematics::setCoordinateDiffInfo(const tnl::Quaternion& rot) {
	tnl::Vector3 tmp = tnl::Vector3::TransformCoord(dk_data_st_.dir_p_to_c_, rot);
	if (is_upd_dk_data_st_) {
		dk_data_st_.len_p_to_c_ = tmp.length();
	}
	tmp.normalize();
	dk_data_st_.dir_p_to_c_ = tmp;
}