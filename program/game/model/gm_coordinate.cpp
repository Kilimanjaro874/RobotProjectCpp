#include "gm_coordinate.h"

void Coordinate::init() {}

void Coordinate::update(float delta_time) {
	directKinematics(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0.00));
	for (auto cod : coordinate_parts_) {
		cod->mesh_->pos_ = getPos() + tnl::Vector3::TransformCoord(cod->ofs_pos_, getRot());
		cod->mesh_->rot_q_ = cod->ofs_rot_ * getRot();
	}
}

void Coordinate::render(dxe::Camera* camera) {
	for (auto cod : coordinate_parts_) {
		if (!cod->is_render_) { continue; }
		cod->mesh_->render(camera);
	}
}

void Coordinate::setCoordinate(
	int id,
	std::string name,
	tnl::Vector3 pos,
	tnl::Vector3 x,
	tnl::Vector3 y,
	tnl::Vector3 z,
	tnl::Vector3 rot_x,
	tnl::Vector3 rot_y,
	tnl::Vector3 rot_z,
	tnl::Quaternion offset_rot
) {

	oc_vec_v_.resize(static_cast<int>(coordinate::end));
	oc_rot_vec_v_.resize(static_cast<int>(coordinate::end));
	id_ = id;
	name_ = name;
	pos_ = pos;
	oc_vec_v_[static_cast<int>(coordinate::x)] = x;
	oc_vec_v_[static_cast<int>(coordinate::y)] = y;
	oc_vec_v_[static_cast<int>(coordinate::z)] = z;
	for (auto ocv : oc_vec_v_) { ocv.normalize(); }
	oc_vec_upd_v_ = oc_vec_v_;
	oc_rot_vec_v_[static_cast<int>(coordinate::x)] = rot_x;
	oc_rot_vec_v_[static_cast<int>(coordinate::y)] = rot_y;
	oc_rot_vec_v_[static_cast<int>(coordinate::z)] = rot_z;
	for (auto ocr : oc_rot_vec_v_) { ocr.normalize(); }
	oc_rot_vec_upd_v_ = oc_rot_vec_v_;
	oc_rot_ = offset_rot;
	oc_rot_upd_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);	// set non rot

	for (int i = 0; i < static_cast<int>(coordinate::end); i++) {
		oc_vec_upd_v_[i] = tnl::Vector3::TransformCoord(oc_vec_v_[i], oc_rot_);
		oc_rot_vec_upd_v_[i] = tnl::Vector3::TransformCoord(oc_rot_vec_v_[i], oc_rot_);
	}
}

void Coordinate::setChildAndDKInit(Coordinate* child, attach_type type) {
	tnl::Vector3 delta;
	if (attach_type::absolute == type) { delta = child->getPos() - pos_; }
	if (attach_type::relative == type) { delta = child->getPos(); }
	float length = delta.length();
	delta.normalize();
	dk_st_ tmp = { child, delta, length };
	children_.push_back(tmp);
}

void Coordinate::directKinematics(tnl::Quaternion rot) {

	// ----- Update self coordinate ----- //
	pos_ = pos_from_parent_;
	rot_from_parent_ *= rot;
	oc_rot_upd_ *= rot_from_parent_;
	
	for (int i = 0; i < static_cast<int>(coordinate::end); i++) {
		oc_vec_upd_v_[i] = tnl::Vector3::TransformCoord(oc_vec_v_[i], oc_rot_upd_);
		oc_rot_vec_upd_v_[i] = tnl::Vector3::TransformCoord(oc_rot_vec_v_[i], oc_rot_upd_);
	}
	// ----- effect of IK ----- //

	tnl::Vector3 tmp_dir;
	tnl::Vector3 tmp_pos;
	for (auto c : children_) {
		tmp_dir = tnl::Vector3::TransformCoord(c.dir_, oc_rot_upd_);
		tmp_pos = pos_ + tmp_dir * c.length_;
		c.coord_->setParentParams(tmp_pos, rot_from_parent_);
	}

	// ----- init ----- //
	pos_from_parent_ = { 0, 0, 0 };
	rot_from_parent_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
}

void Coordinate::setIKObjectTargetInit(Coordinate* object, Coordinate* target, ik_type type, float kp,
	bool is_rot_axis_x, bool is_rot_axis_y, bool is_rot_axis_z ) {
	ik_st_ tmp = { object, target, type, kp, is_rot_axis_x, is_rot_axis_y, is_rot_axis_z };
	ik_settings_.push_back(tmp);
}

tnl::Quaternion Coordinate::inverseKinematics() {

	return tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
}

// ----- setter, getter ----- //
void Coordinate::setTreeLocateInfo(int type, int col, int row) {
	tree_st_data_.com_type = type;
	tree_st_data_.com_col = col;
	tree_st_data_.com_row = row;
}

void Coordinate::setViewCoorinate(float radius, float length) {
	Parts* x = new Parts();
	x->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	x->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	x->ofs_pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::x)] * length / 2;
	x->ofs_rot_ = oc_rot_ * tnl::Quaternion::RotationAxis(oc_rot_vec_upd_v_[static_cast<int>(coordinate::z)], tnl::ToRadian(90));
	coordinate_parts_.push_back(x);
	Parts* y = new Parts();
	y->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	y->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	y->ofs_pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::y)] * length / 2;
	y->ofs_rot_ = oc_rot_ * tnl::Quaternion::RotationAxis(oc_rot_vec_upd_v_[static_cast<int>(coordinate::z)], tnl::ToRadian(0));
	coordinate_parts_.push_back(y);
	Parts* z = new Parts();
	z->mesh_ = dxe::Mesh::CreateCylinder(radius, length);
	z->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	z->ofs_pos_ = oc_vec_upd_v_[static_cast<int>(coordinate::z)] * length / 2;
	z->ofs_rot_ = oc_rot_ * tnl::Quaternion::RotationAxis(oc_rot_vec_upd_v_[static_cast<int>(coordinate::x)], tnl::ToRadian(90));
	coordinate_parts_.push_back(z);
}

