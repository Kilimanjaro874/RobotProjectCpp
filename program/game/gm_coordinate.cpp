#include "gm_coordinate.h"

void tol::Coordinate::update(float delta_time) {
	for (auto m : mesh_v_) {
		m->pos_ = pos_ +tnl::Vector3::TransformCoord(m->pos_, rot_);
		m->rot_q_ = m->rot_q_ * rot_;
	}
}

void tol::Coordinate::render(dxe::Camera* camera) {
	for (auto m : mesh_v_) {
		if (!is_render_) {
			continue;
		}
		m->render(camera);
	}
}

/// <summary>
/// Set Coordinate data.
/// </summary>
/// <param name="pos"> origine (from World origine:ƒ°0)</param>
/// <param name="rot"> rotate(ƒ°0) </param>
/// <param name="dir_x"> orthonormal basis x </param>
/// <param name="dir_y"> orthonormal basis y </param>
/// <param name="dir_z"> orthonormal basis z </param>
void tol::Coordinate::setCoordinate(tnl::Vector3 pos, tnl::Quaternion rot,
	tnl::Vector3 dir_x, tnl::Vector3 dir_y, tnl::Vector3 dir_z) {
	pos_ = pos;
	rot_ = rot;
	oc_vec_v_.resize(static_cast<int>(coordinate::end));
	oc_vec_v_[static_cast<int>(coordinate::x)] = dir_x;
	oc_vec_v_[static_cast<int>(coordinate::y)] = dir_y;
	oc_vec_v_[static_cast<int>(coordinate::z)] = dir_z;
	for (auto oc : oc_vec_v_) {
		oc = tnl::Vector3::TransformCoord(oc, rot);
		oc.normalize();
	}
	oc_vec_upd_v_ = oc_vec_v_;
}

/// <summary>
/// If you want to visualize the coordinates: call this func.
/// </summary>
/// <param name="length"> visualize coordinate length x, y, z </param>
/// <param name="radius"> the cylinder thickness </param>
void tol::Coordinate::setViewCoordinate(float length, float radius) {
	is_render_ = true;
	dxe::Mesh* x = dxe::Mesh::CreateCylinder(radius, length);
	x->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	x->pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::x)] * length / 2;
	x->rot_q_ = rot_ * tnl::Quaternion::RotationAxis(oc_vec_upd_v_[static_cast<int>(coordinate::z)], tnl::ToRadian(90));
	mesh_v_.push_back(x);
	dxe::Mesh* y = dxe::Mesh::CreateCylinder(radius, length);
	y->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	y->pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::y)] * length / 2;
	y->rot_q_ = rot_ * tnl::Quaternion::RotationAxis(oc_vec_upd_v_[static_cast<int>(coordinate::z)], tnl::ToRadian(0));
	mesh_v_.push_back(y);
	dxe::Mesh* z = dxe::Mesh::CreateCylinder(radius, length);
	z->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	z->pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::z)] * length / 2;
	z->rot_q_ = rot_ * tnl::Quaternion::RotationAxis(oc_vec_upd_v_[static_cast<int>(coordinate::x)], tnl::ToRadian(90));
	mesh_v_.push_back(z);
}

/// <summary>
/// Update Coordinate by rot(Quaternion : additional rot)
/// </summary>
/// <param name="rot"> additional Quaternion </param>
void tol::Coordinate::setAddRot(tnl::Quaternion rot) {
	rot_ *= rot;
	for (int i = 0; i < static_cast<int>(coordinate::end); i++) {
		oc_vec_upd_v_[i] = tnl::Vector3::TransformCoord(oc_vec_v_[i], rot_);
	}
}