#include "gm_faceVec.h"

void FaceVec::update(float delta_time) {
	//dir_z_ = tnl::Vector3::TransformCoord(dir_z_, rot_sum_);
	//dir_x_ = tnl::Vector3::TransformCoord(dir_x_, rot_sum_);
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
	// --- 制御目標 or 制御対象 位置・姿勢を可視化するためのパーツ用意 --- //
	FaceVec* fv = new FaceVec();
	fv->parts_.resize(e_parts_max);
	fv->pos_ = pos_;
	fv->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	fv->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	// -- 球の表示 -- /
	Parts* sp_p = new Parts();
	sp_p->mesh_ = dxe::Mesh::CreateSphere(2);
	sp_p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	// 位置・姿勢設定
	sp_p->ofs_pos_ = { 0, 0, 0 };
	// -- 方向の表示 (z) -- 
	Parts* cy_pz = new Parts();
	cy_pz->mesh_ = dxe::Mesh::CreateCylinder(1.0, 20);
	cy_pz->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red1.bmp"));
	// 位置・姿勢設定
	cy_pz->ofs_pos_ += tnl::Vector3{ 0, 0, 10 };
	cy_pz->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	// -- 方向の表示(x) --
	Parts* cy_px = new Parts();
	cy_px->mesh_ = dxe::Mesh::CreateCylinder(1.0, 20);
	cy_px->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	// 位置・姿勢設定
	cy_px->ofs_pos_ += tnl::Vector3{ 10, 0, 0 };
	cy_px->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	// --- 部品を登録 --- //
	fv->parts_[e_origine_] = sp_p;
	fv->parts_[e_dir_z_] = cy_pz;
	fv->parts_[e_dir_x_] = cy_px;

	return fv;
}

void FaceVec::Rotate(tnl::Quaternion rot_temp) {
	// --- 1フレーム間の回転量変化：rot_tempの影響を本クラスへ反映 --- //
	for (auto pts : parts_) {
		rot_sum_ *= rot_temp;
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}

}

