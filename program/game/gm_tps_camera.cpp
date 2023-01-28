#include "gm_tps_camera.h"

void tol::TPSCamera::camUpdate(std::shared_ptr<tol::Object> obj, const tnl::Vector3& offset_pos)
{
	auto cod = getCoordinate();
	auto target_cod = obj->getCoordinate();
	view_ = tnl::Matrix::LookAtLH(cod->getPos(), target_cod->getPos() + offset_pos, up_);
	prj_ = tnl::Matrix::PerspectiveFovLH(angle_, aspect_, near_, far_);
}
