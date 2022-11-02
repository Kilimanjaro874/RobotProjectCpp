#include "gm_face_vector.h"
#include "gm_module.h"

FaceVector* FaceVector::Create(int id, tnl::Vector3 pos, tnl::Quaternion rot) {
	// ----- ����ڕW or ����Ώۂ̈ʒu�E�p�����������邽�߂̃p�[�c�p�� ------ //
	FaceVector* fv = new FaceVector();
	fv->id_ = id;
	// ---- ���W�n�ݒ� ---- //
	fv->parts_.resize(e_parts_max);
	fv->in_rot_axis_ = { 0, 1, 0 };
	fv->in_dir_z_ = { 0, 0, 1 };
	fv->in_dir_x_ = { 1, 0, 1 };
	// --- DK���{ --- //
	fv->SelfDK(pos, rot);

	// ---- �`��p�p�[�c�p�� ---- //
	// -- �� -- /
	Parts* sp_p = new Parts();
	sp_p->mesh_ = dxe::Mesh::CreateSphere(fv->sphere_size_);
	sp_p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	sp_p->ofs_pos_ = { 0, 0, 0 };
	// -- �����̕\�� (z) -- 
	Parts* cy_pz = new Parts();
	cy_pz->mesh_ = dxe::Mesh::CreateCylinder(0.1, fv->bar_length_);
	cy_pz->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	// �ʒu�E�p���ݒ�
	cy_pz->ofs_pos_ += tnl::Vector3{ 0, 0, fv->bar_length_/2 };
	cy_pz->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	// -- �����̕\��(x) --
	Parts* cy_px = new Parts();
	cy_px->mesh_ = dxe::Mesh::CreateCylinder(0.1, fv->bar_length_);
	cy_px->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	// �ʒu�E�p���ݒ�
	cy_px->ofs_pos_ += tnl::Vector3{ fv->bar_length_/2, 0, 0 };
	cy_px->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	// --- ���i��o�^ --- //
	fv->parts_[e_origine] = sp_p;
	fv->parts_[e_dir_z] = cy_pz;
	fv->parts_[e_dir_x] = cy_px;
	// ----- ���i�ʒu�E�p���A�b�v�f�[�g ----- //
	fv->update(0);

	return fv;

}