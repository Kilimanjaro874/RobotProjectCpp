#include "gm_airplane.h"


MdlAirPlane* MdlAirPlane::Create() {

	MdlAirPlane* mdl = new MdlAirPlane();
	mdl->parts_.resize(e_parts_max);

	Parts* body = new Parts();
	body->mesh_ = dxe::Mesh::CreateCylinder(5, 70);
	body->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	body->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	mdl->parts_[e_body] = body;

	Parts* r_front_wing = new Parts();
	r_front_wing->mesh_ = dxe::Mesh::CreateTriangleRight({ 25, 40, 0 }, 10, 10, false);
	r_front_wing->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	r_front_wing->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	r_front_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));
	r_front_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	r_front_wing->ofs_pos_ = { 2.5, 0, 10 };
	mdl->parts_[e_r_front_wing] = r_front_wing;


	Parts* l_front_wing = new Parts();
	l_front_wing->mesh_ = dxe::Mesh::CreateTriangleRight({ 25, 40, 0 });
	l_front_wing->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	l_front_wing->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(180));
	l_front_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	l_front_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	l_front_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	l_front_wing->ofs_pos_ = { 2.5, 0, 10 };
	mdl->parts_[e_l_front_wing] = l_front_wing;


	Parts* r_back_wing = new Parts();
	r_back_wing->mesh_ = dxe::Mesh::CreateTriangleRight({ 15, 20, 0 }, 10, 10, false);
	r_back_wing->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	r_back_wing->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	r_back_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));
	r_back_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	r_back_wing->ofs_pos_ = { 2.5, 0, -30 };
	mdl->parts_[e_r_back_wing] = r_back_wing;


	Parts* l_back_wing = new Parts();
	l_back_wing->mesh_ = dxe::Mesh::CreateTriangleRight({ 15, 20, 0 });
	l_back_wing->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	l_back_wing->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(180));
	l_back_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	l_back_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	l_back_wing->ofs_rot_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	l_back_wing->ofs_pos_ = { 2.5, 0, -30 };
	mdl->parts_[e_l_back_wing] = l_back_wing;

	Parts* t_back_wing = new Parts();
	t_back_wing->mesh_ = dxe::Mesh::CreateTriangleRight({ 10, 15, 0 });
	t_back_wing->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	t_back_wing->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	t_back_wing->ofs_pos_ = { 0, 2.5, -30 };
	mdl->parts_[e_t_back_wing] = t_back_wing;

	Parts* gyro = new Parts();
	gyro->mesh_ = dxe::Mesh::CreateCone(10, 10);
	gyro->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	gyro->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	gyro->ofs_pos_ = { 0, 0, 35 };
	mdl->parts_[e_gyro] = gyro;

	Parts* propeller = new Parts();
	propeller->mesh_ = dxe::Mesh::CreatePlane({ 30, 2, 0 });
	propeller->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	propeller->ofs_pos_ = { 0, 0, 40 };
	mdl->parts_[e_propeller] = propeller;

	mdl->update(0);

	return mdl;
}



void MdlAirPlane::update(float delta_time) {
	Model::update( delta_time );

	propeller_angle_ += tnl::ToRadian(30);
	parts_[e_propeller]->mesh_->rot_q_ = rot_;
	tnl::Vector3 axis = tnl::Vector3::TransformCoord({ 0, 0, 1 }, rot_);
	parts_[e_propeller]->mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(axis, propeller_angle_);
}
