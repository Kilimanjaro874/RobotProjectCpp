#include "gm_faceVec.h"

void FaceVec::update(float delta_time) {
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void FaceVec::render(dxe::Camera* camera) {
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

FaceVec* FaceVec::Create(tnl::Vector3 pos_) {
	// --- ����ڕW or ����Ώ� �ʒu�E�p�����������邽�߂̃p�[�c�p�� --- //
	FaceVec* fv = new FaceVec();
	fv->parts_.resize(e_parts_max);
	fv->pos_ = pos_;
	fv->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	fv->init_dir_z_ = fv->dir_z_;
	fv->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	fv->init_dir_x_ = fv->dir_x_;
	// -- ���̕\�� -- /
	Parts* sp_p = new Parts();
	sp_p->mesh_ = dxe::Mesh::CreateSphere(2);
	sp_p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	// �ʒu�E�p���ݒ�
	sp_p->ofs_pos_ = { 0, 0, 0 };
	// -- �����̕\�� (z) -- 
	Parts* cy_pz = new Parts();
	cy_pz->mesh_ = dxe::Mesh::CreateCylinder(1.0, 20);
	cy_pz->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	// �ʒu�E�p���ݒ�
	cy_pz->ofs_pos_ += tnl::Vector3{ 0, 0, 10 };
	cy_pz->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	// -- �����̕\��(x) --
	Parts* cy_px = new Parts();
	cy_px->mesh_ = dxe::Mesh::CreateCylinder(1.0, 20);
	cy_px->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	// �ʒu�E�p���ݒ�
	cy_px->ofs_pos_ += tnl::Vector3{ 10, 0, 0 };
	cy_px->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	// --- ���i��o�^ --- //
	fv->parts_[e_origine_] = sp_p;
	fv->parts_[e_dir_z_] = cy_pz;
	fv->parts_[e_dir_x_] = cy_px;

	return fv;
}



void FaceVec::Rotate(tnl::Quaternion rot_temp) {
	// --- 1�t���[���Ԃ̉�]�ʕω��Frot_temp�̉e����{�N���X�֔��f --- //
	rot_sum_ *= rot_temp;
	dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	dir_x_ = tnl::Vector3::TransformCoord(init_dir_x_, rot_sum_);
	dir_r_ = tnl::Vector3::TransformCoord(init_dir_r_, rot_sum_);
}

void FaceVec::InitDK(tnl::Vector3 refPos) {
	// --- DK�ŎQ�Ƃ�����W���w�� --- //
	dir_r_ = pos_ - refPos;
	r_length_ = dir_r_.length();
	dir_r_.normalize();
	init_dir_r_ = dir_r_;
}

void FaceVec::DirectKinematics(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// --- �Q�Ƃ�����W�n����̈ʒu�E1�t���[���Ԃ̉�]���󂯎��A�ʒu�X�V����֐� ---
	// -- ��]�n�̍X�V -- //
	Rotate(q_back);
	pos_ = p_back + dir_r_ * r_length_;
}
