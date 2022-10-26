#include "gm_face_vector.h"
#include "gm_module.h"

FaceVector* FaceVector::Create(int id, tnl::Vector3 pos, tnl::Quaternion rot) {
	// ----- 制御目標 or 制御対象の位置・姿勢を可視化するためのパーツ用意 ------ //
	FaceVector* fv = new FaceVector();
	fv->id_ = id;
	// ---- 座標系設定 ---- //
	fv->parts_.resize(e_parts_max);
	fv->in_rot_axis_ = { 0, 1, 0 };
	fv->in_dir_z_ = { 0, 0, 1 };
	fv->in_dir_x_ = { 1, 0, 1 };
	// --- DK実施 --- //
	fv->SelfDK(pos, rot);

	// ---- 描画用パーツ用意 ---- //
	// -- 球 -- /
	Parts* sp_p = new Parts();
	sp_p->mesh_ = dxe::Mesh::CreateSphere(fv->sphere_size_);
	sp_p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	sp_p->ofs_pos_ = { 0, 0, 0 };
	// -- 方向の表示 (z) -- 
	Parts* cy_pz = new Parts();
	cy_pz->mesh_ = dxe::Mesh::CreateCylinder(0.1, fv->bar_length_);
	cy_pz->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	// 位置・姿勢設定
	cy_pz->ofs_pos_ += tnl::Vector3{ 0, 0, fv->bar_length_/2 };
	cy_pz->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	// -- 方向の表示(x) --
	Parts* cy_px = new Parts();
	cy_px->mesh_ = dxe::Mesh::CreateCylinder(0.1, fv->bar_length_);
	cy_px->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	// 位置・姿勢設定
	cy_px->ofs_pos_ += tnl::Vector3{ fv->bar_length_/2, 0, 0 };
	cy_px->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	// --- 部品を登録 --- //
	fv->parts_[e_origine] = sp_p;
	fv->parts_[e_dir_z] = cy_pz;
	fv->parts_[e_dir_x] = cy_px;
	// ----- 部品位置・姿勢アップデート ----- //
	fv->update(0);

	return fv;

}