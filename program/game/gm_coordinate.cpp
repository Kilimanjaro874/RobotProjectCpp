#include "gm_coordinate.h"

void tol::Coordinate::init(tnl::Vector3 pos, tnl::Quaternion rot,
	tnl::Vector3 dir_x, tnl::Vector3 dir_y, tnl::Vector3 dir_z) {
	pos_ = pos;
	rot_ = rot;
	c_vec_v_.resize(static_cast<int>(coordinate::end));
	c_vec_v_[static_cast<int>(coordinate::x)] = dir_x;
	c_vec_v_[static_cast<int>(coordinate::y)] = dir_y;
	c_vec_v_[static_cast<int>(coordinate::z)] = dir_z;
	for (auto oc : c_vec_v_) {
		oc = tnl::Vector3::TransformCoord(oc, rot);
		oc.normalize();
	}
	c_vec_upd_v_ = c_vec_v_;
}

void tol::Coordinate::setAddRot(const tnl::Quaternion& rot) {
	rot_ *= rot;
	for (int i = 0; i < static_cast<int>(coordinate::end); i++) {
		c_vec_upd_v_[i] = tnl::Vector3::TransformCoord(c_vec_v_[i], rot_);
	}
}

/// <summary>
/// Return a copy of this Coordinate Class
/// </summary>
/// <returns> Copied this class </returns>
std::shared_ptr<tol::Coordinate> tol::Coordinate::copyCoordinate() {
	std::shared_ptr<Coordinate> tmp_cod = std::make_shared<Coordinate>(Coordinate());
	tmp_cod->init(getPos(), getRot(), getDirX(), getDirY(), getDirZ());
	return tmp_cod;
}