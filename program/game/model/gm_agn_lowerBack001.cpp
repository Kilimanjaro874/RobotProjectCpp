#include "gm_agn_lowerBack001.h"

Agn_lowerBack001* Agn_lowerBack001::Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- エージェントが管理するモジュールをインスタンス化＆参照リストに追加 ----- //
	Agn_lowerBack001* agn = new Agn_lowerBack001();
	agn->modules_.resize(e_modules_max);
	agn->init_rot_axis_ = tnl::Vector3{ 0, 1, 0 };
	agn->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	agn->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	agn->link_length_ = 0;

	// ----- 各モジュールの設定を実施：初期状態 mode01に設定 ----- //
	// ---- y, x, zの3DOF球状関節を定義する ---- //
	// ---- 1. e_bb_y ---- //
	Module* bb_y = new Module();
	bb_y->id_ = 1;
	bb_y->init_rot_axis_ = tnl::Vector3{ 0, 1, 0 };
	bb_y->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	bb_y->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	bb_y->rot_sum_ = tnl::Quaternion::RotationAxis(bb_y->init_rot_axis_, 0);
	bb_y->link_length_ = 0;
	agn->modules_[e_bb_y] = bb_y;

	// ---- 2. e_bb_x ---- //
	Module* bb_x = new Module();
	bb_x->id_ = 2;
	bb_x->init_rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	bb_x->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	bb_x->init_dir_x_ = tnl::Vector3{ 0, 1, 0 };
	bb_x->rot_sum_ = tnl::Quaternion::RotationAxis(bb_x->init_rot_axis_, 0);
	bb_x->link_length_ = 0;
	agn->modules_[e_bb_x] = bb_x;

	// ---- 3. e_bb_z ---- //
	Module* bb_z = new Module();
	bb_z->id_ = 3;
	bb_z->init_rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	bb_z->init_dir_z_ = tnl::Vector3{ 0, 1, 0 };
	bb_z->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	bb_z->rot_sum_ = tnl::Quaternion::RotationAxis(bb_z->init_rot_axis_, 0);
	bb_z->link_length_ = agn->backBone_length_;
	Parts* bb_z_bone = new Parts();
	bb_z_bone->mesh_ = dxe::Mesh::CreateCylinder(5, agn->backBone_length_);
	bb_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	Parts* bbz_hp = new Parts();
	bbz_hp->mesh_ = dxe::Mesh::CreateCylinder(5, agn->hipJoint_length_);
	bbz_hp->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bbz_hp->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	bbz_hp->ofs_pos_ += bb_z->init_dir_z_ * -1 * agn->backBone_length_ / 2;
	bb_z->parts_.push_back(bb_z_bone);
	bb_z->parts_.push_back(bbz_hp);
	agn->modules_[e_bb_z] = bb_x;
	
	// ---- 上記初期位置。姿勢を反映 ---- //
	agn->DirectKinematics_world(p_back, q_back);
	tnl::Vector3 tmp_p_back = agn->pos_o_next_;
	tnl::Quaternion tmp_q_back = agn->rot_tmp_;
	for (auto mod : agn->modules_) {
		mod->DirectKinematics_world(tmp_p_back, tmp_q_back);
		tmp_p_back = mod->pos_o_next_;
		tmp_q_back = mod->rot_tmp_;
	}
	// ---- 部品の位置・姿勢を順運動学に応じて反映 ---- //
	agn->update(0);

	return agn;
}


void Agn_lowerBack001::mode01_init(const std::vector<FaceVec*> targets) {
	// ----- ターゲットに向かってエイム動作：初期化 ----- //
	// ターゲットの数：1つ ()
	// targets[0] : 腰を傾けたい時、本target操作
	
	// ----- 制御対象のセッティング ----- //
	cnt_objects_.clear();
	cnt_objects_.resize(1);
	cnt_objects_[0] = FaceVec::Create({ 0, 0, 0 });		// 

	// ----- 各モジュールのik_setting実施
	// --- 1. e_bb_y --- //
	modules_[e_bb_y]->ik_settings_.clear();
	modules_[e_bb_y]->ik_settings_.resize(1);
	modules_[e_bb_y]->ik_settings_[0] = { dirz_to_pos, 0.2, targets[0], cnt_objects_[0] };
	// --- 2. e_bb_x --- //
	modules_[e_bb_x]->ik_settings_.clear();
	modules_[e_bb_x]->ik_settings_.resize(1);
	modules_[e_bb_x]->ik_settings_[0] = { dirx_to_pos, 0.2, targets[0], cnt_objects_[0] };
	// --- 1. e_bb_z --- //
	modules_[e_bb_z]->ik_settings_.clear();
	modules_[e_bb_z]->ik_settings_.resize(1);
	modules_[e_bb_z]->ik_settings_[0] = { dirz_to_pos, 0.2, targets[0], cnt_objects_[0] };

}


void Agn_lowerBack001::mode01_update(float delta_time, const tnl::Vector3& p_back,
	const tnl::Quaternion& q_back, std::vector<FaceVec*> targets) {
	// ----- ターゲットに向かってエイム動作：IK更新 ----- //
	// ターゲットの数：2つ (照準位置、肘引きの目標位置)
	// targets[0] : 腰を傾けたい時、本target操作
	// ---- 制御目標の更新 ---- //
	targets_.resize(1);
	targets_[0] = targets[0];

	// ---- 制御対象の更新 ---- //
	cnt_objects_[0]->pos_ = modules_[e_bb_z]->pos_o_next_;	// 腰位置更新
	cnt_objects_[0]->Rotate(modules_[e_bb_z]->rot_tmp_);

	tnl::Vector3 tmp_pos = p_back;
	tnl::Quaternion tmp_q = q_back;
	for (auto mod : modules_) {
		mod->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
		tmp_pos = mod->pos_o_next_;
		tmp_q = mod->rot_tmp_;
	}

	// 描画系アップデート
	update(delta_time);
}

void Agn_lowerBack001::mode02_init(FaceVec& target) {

}

void Agn_lowerBack001::mode02_update() {

}