#include "gm_agn_armR001.h"

Agn_armR001* Agn_armR001::Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- エージェントが管理するモジュールをインスタンス化＆参照リストに追加 ----- //
	Agn_armR001* agn = new Agn_armR001();
	agn->modules_.resize(e_modules_max);
	agn->init_rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	agn->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	agn->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	agn->link_length_ = 0;

	// ----- 各モジュールの設定を実施：初期状態 mode01に設定 ----- //
	// ---- 1. e_sho_x ---- //
	Module* sho_x = new Module();
	sho_x->id_ = 1;
	sho_x->init_rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	sho_x->init_dir_z_ =	tnl::Vector3{ 0, 0, 1 };
	sho_x->init_dir_x_ =	tnl::Vector3{ 1, 0, 0 };
	sho_x->rot_sum_ = tnl::Quaternion::RotationAxis(sho_x->init_rot_axis_, 0);
	sho_x->link_length_ = 0;
	Parts* sho_x_rot01 = new Parts();
	sho_x_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 4);
	sho_x_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	sho_x_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	Parts* sho_x_lid01 = new Parts();
	sho_x_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_x_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	sho_x_lid01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));
	sho_x_lid01->ofs_pos_ += tnl::Vector3{ 2.0, 0, 0 };
	Parts* sho_x_lid02 = new Parts();
	sho_x_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_x_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	sho_x_lid02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	sho_x_lid02->ofs_pos_ += tnl::Vector3{ -2.0, 0, 0 };
	Parts* sho_x_dirx01 = new Parts();
	sho_x_dirx01->mesh_ = dxe::Mesh::CreateCylinder(2, 10);
	sho_x_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	sho_x_dirx01->ofs_pos_ += tnl::Vector3{ 5, 0, 0 };
	sho_x_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	sho_x->parts_.push_back(sho_x_rot01);
	sho_x->parts_.push_back(sho_x_lid01);
	sho_x->parts_.push_back(sho_x_lid02);
	sho_x->parts_.push_back(sho_x_dirx01);
	agn->modules_[e_sho_x] = sho_x;

	// ---- 2. e_sho_y ---- //
	Module* sho_y = new Module();
	sho_y->id_ = 2;
	sho_y->init_rot_axis_ = tnl::Vector3{ 0, 1, 0 };
	sho_y->init_dir_z_ =	tnl::Vector3{ 0, 0, 1 };
	sho_y->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	sho_y->rot_sum_ = tnl::Quaternion::RotationAxis(sho_y->init_rot_axis_, 0);
	sho_y->link_length_ = 30;
	Parts* sho_y_ln01 = new Parts();
	sho_y_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.0, 20);
	sho_y_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue_green.bmp"));
	sho_y_ln01->ofs_pos_ += sho_y->init_dir_z_ * sho_y->link_length_ / 2;
	sho_y_ln01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	Parts* sho_y_rot01 = new Parts();
	sho_y_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 4);
	sho_y_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	Parts* sho_y_lid01 = new Parts();
	sho_y_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_y_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	sho_y_lid01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	sho_y_lid01->ofs_pos_ += tnl::Vector3{ 0, 2.0, 0 };
	Parts* sho_y_lid02 = new Parts();
	sho_y_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_y_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	sho_y_lid02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	sho_y_lid02->ofs_pos_ += tnl::Vector3{ 0, -2.0, 0 };
	Parts* sho_y_dirx01 = new Parts();
	sho_y_dirx01->mesh_ = dxe::Mesh::CreateCylinder(1, 15);
	sho_y_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	sho_y_dirx01->ofs_pos_ += tnl::Vector3{ 7.5, 0, 0 };
	sho_y_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	sho_y->parts_.push_back(sho_y_ln01);
	sho_y->parts_.push_back(sho_y_rot01);
	sho_y->parts_.push_back(sho_y_lid01);
	sho_y->parts_.push_back(sho_y_lid02);
	sho_y->parts_.push_back(sho_y_dirx01);
	agn->modules_[e_sho_y] = sho_y;

	// ---- 3. e_arm_x ---- //
	Module* arm_x = new Module();
	arm_x->id_ = 3;
	arm_x->init_rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	arm_x->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	arm_x->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	arm_x->rot_sum_ = tnl::Quaternion::RotationAxis(arm_x->init_rot_axis_, 0);
	arm_x->link_length_ = 0;
	Parts* arm_x_rot01 = new Parts();
	arm_x_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 4);
	arm_x_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	arm_x_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	Parts* arm_x_lid01 = new Parts();
	arm_x_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_x_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	arm_x_lid01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));
	arm_x_lid01->ofs_pos_ += tnl::Vector3{ 2.0, 0, 0 };
	Parts* arm_x_lid02 = new Parts();
	arm_x_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_x_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	arm_x_lid02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	arm_x_lid02->ofs_pos_ += tnl::Vector3{ -2.0, 0, 0 };
	Parts* arm_x_dirx01 = new Parts();
	arm_x_dirx01->mesh_ = dxe::Mesh::CreateCylinder(2, 10);
	arm_x_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	arm_x_dirx01->ofs_pos_ += tnl::Vector3{ 5, 0, 0 };
	arm_x_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	arm_x->parts_.push_back(arm_x_rot01);
	arm_x->parts_.push_back(arm_x_lid01);
	arm_x->parts_.push_back(arm_x_lid02);
	arm_x->parts_.push_back(arm_x_dirx01);
	agn->modules_[e_arm_x] = arm_x;

	// ---- 4. e_arm_z ---- //
	Module* arm_z = new Module();
	arm_z->id_ = 4;
	arm_z->init_rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	arm_z->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	arm_z->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	arm_z->rot_sum_ = tnl::Quaternion::RotationAxis(arm_z->init_rot_axis_, 0);
	arm_z->link_length_ = 20;
	Parts* arm_z_ln01 = new Parts();
	arm_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	arm_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red_blue.bmp"));
	arm_z_ln01->ofs_pos_ += arm_z->init_dir_z_ * arm_z->link_length_ / 2;
	arm_z_ln01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	Parts* arm_z_rot01 = new Parts();
	arm_z_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	arm_z_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	arm_z_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* arm_z_lid01 = new Parts();
	arm_z_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_z_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	arm_z_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* arm_z_lid02 = new Parts();
	arm_z_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_z_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	arm_z_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };
	Parts* arm_z_dirx01 = new Parts();
	arm_z_dirx01->mesh_ = dxe::Mesh::CreateCylinder(1, 15);
	arm_z_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	arm_z_dirx01->ofs_pos_ += tnl::Vector3{ 7.5, 0, 0 };
	arm_z_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	arm_z->parts_.push_back(arm_z_ln01);
	arm_z->parts_.push_back(arm_z_rot01);
	arm_z->parts_.push_back(arm_z_lid01);
	arm_z->parts_.push_back(arm_z_lid02);
	arm_z->parts_.push_back(arm_z_dirx01);
	agn->modules_[e_arm_z] = arm_z;

	// ---- 5. e_wrist_z ---- //
	Module* wrist_z = new Module();
	wrist_z->id_ = 5;
	wrist_z->init_rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_z->rot_sum_ = tnl::Quaternion::RotationAxis(wrist_z->init_rot_axis_, 0);
	wrist_z->link_length_ = 10;
	Parts* wrist_z_ln01 = new Parts();
	wrist_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 10);
	wrist_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_ln01->ofs_pos_ += wrist_z->init_dir_z_ * wrist_z->link_length_ / 2;
	wrist_z_ln01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	Parts* wrist_z_rot01 = new Parts();
	wrist_z_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	wrist_z_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* wrist_z_lid01 = new Parts();
	wrist_z_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* wrist_z_lid02 = new Parts();
	wrist_z_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };
	Parts* wrist_z_dirx01 = new Parts();
	wrist_z_dirx01->mesh_ = dxe::Mesh::CreateCylinder(2, 10);
	wrist_z_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_dirx01->ofs_pos_ += tnl::Vector3{ 5, 0, 0 };
	wrist_z_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	wrist_z->parts_.push_back(wrist_z_ln01);
	wrist_z->parts_.push_back(wrist_z_rot01);
	wrist_z->parts_.push_back(wrist_z_lid01);
	wrist_z->parts_.push_back(wrist_z_lid02);
	wrist_z->parts_.push_back(wrist_z_dirx01);
	agn->modules_[e_wrist_z] = wrist_z;

	// ---- 6. e_wrist_x ---- //
	Module* wrist_x = new Module();
	wrist_x->id_ = 6;
	wrist_x->init_rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	wrist_x->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_x->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_x->rot_sum_ = tnl::Quaternion::RotationAxis(wrist_x->init_rot_axis_, 0);
	wrist_x->link_length_ = 0;
	Parts* wrist_x_rot01 = new Parts();
	wrist_x_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 4);
	wrist_x_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	wrist_x_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	Parts* wrist_x_lid01 = new Parts();
	wrist_x_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_x_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	wrist_x_lid01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(90));
	wrist_x_lid01->ofs_pos_ += tnl::Vector3{ 2.0, 0, 0 };
	Parts* wrist_x_lid02 = new Parts();
	wrist_x_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_x_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	wrist_x_lid02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-90));
	wrist_x_lid02->ofs_pos_ += tnl::Vector3{ -2.0, 0, 0 };
	Parts* wrist_x_dirx01 = new Parts();
	wrist_x_dirx01->mesh_ = dxe::Mesh::CreateCylinder(2, 10);
	wrist_x_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	wrist_x_dirx01->ofs_pos_ += tnl::Vector3{ 5, 0, 0 };
	wrist_x_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	wrist_x->parts_.push_back(wrist_x_rot01);
	wrist_x->parts_.push_back(wrist_x_lid01);
	wrist_x->parts_.push_back(wrist_x_lid02);
	wrist_x->parts_.push_back(wrist_x_dirx01);
	agn->modules_[e_wrist_x] = wrist_x;

	// ---- 7. e_wrist_z22 ---- //
	Module* wrist_z2 = new Module();
	wrist_z2->id_ = 7;
	wrist_z2->init_rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z2->init_dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z2->init_dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_z2->rot_sum_ = tnl::Quaternion::RotationAxis(wrist_z2->init_rot_axis_, 0);
	wrist_z2->link_length_ = 10;
	Parts* wrist_z2_ln01 = new Parts();
	wrist_z2_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 10);
	wrist_z2_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_ln01->ofs_pos_ += wrist_z2->init_dir_z_ * wrist_z2->link_length_ / 2;
	wrist_z2_ln01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	Parts* wrist_z2_rot01 = new Parts();
	wrist_z2_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	wrist_z2_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* wrist_z2_lid01 = new Parts();
	wrist_z2_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z2_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* wrist_z2_lid02 = new Parts();
	wrist_z2_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z2_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };
	Parts* wrist_z2_dirx01 = new Parts();
	wrist_z2_dirx01->mesh_ = dxe::Mesh::CreateCylinder(1, 15);
	wrist_z2_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_dirx01->ofs_pos_ += tnl::Vector3{ 7.5, 0, 0 };
	wrist_z2_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	wrist_z2->parts_.push_back(wrist_z2_ln01);
	wrist_z2->parts_.push_back(wrist_z2_rot01);
	wrist_z2->parts_.push_back(wrist_z2_lid01);
	wrist_z2->parts_.push_back(wrist_z2_lid02);
	wrist_z2->parts_.push_back(wrist_z2_dirx01);
	agn->modules_[e_wrist_z2] = wrist_z2;

	// ---- 上記初期位置・姿勢を反映 ---- //
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

void Agn_armR001::mode01_init(const std::vector<FaceVec*> targets){
	// ----- ターゲットに向かってエイム動作：初期化 ----- //
	// ターゲットの数：2つ (照準位置、肘引きの目標位置)
	// targets[0] : 照準位置
	// targets[1] ： 肘引きの目標位置
	
	// ----- 制御対象のセッティング ----- //
	cnt_objects_.clear();
	cnt_objects_.resize(2);
	cnt_objects_[0] = FaceVec::Create({ 0, 0, 0 });		// 手先位置・姿勢
	cnt_objects_[1] = FaceVec::Create({ 0, 0, 0 });		// 肘の位置・姿勢

	// ----- 各モジュールのik_setting実施
	// --- 1. e_sho_x --- //
	modules_[e_sho_x]->ik_settings_.clear();
	modules_[e_sho_x]->ik_settings_.resize(2);
	modules_[e_sho_x]->ik_settings_[0] = { pos_to_pos, 0.4, targets[0], cnt_objects_[0] };		// 標的位置への位置IK
	modules_[e_sho_x]->ik_settings_[1] = { pos_to_pos, 0.4, targets[1], cnt_objects_[1] };		// 肘位置への位置IK
	// --- 2. e_sho_y --- //
	modules_[e_sho_y]->ik_settings_.clear();
	modules_[e_sho_y]->ik_settings_.resize(1);
	modules_[e_sho_y]->ik_settings_[0] = { pos_to_pos, 0.4, targets[1], cnt_objects_[1] };		// 肘位置への位置IK
	// --- 3. e_arm_x --- //
	modules_[e_arm_x]->ik_settings_.clear();
	modules_[e_arm_x]->ik_settings_.resize(1);
	modules_[e_arm_x]->ik_settings_[0] = { pos_to_pos, 0.4, targets[0], cnt_objects_[0] };		// 標的位置への位置IK
	// --- 4. e_arm_z --- //
	modules_[e_arm_z]->ik_settings_.clear();
	modules_[e_arm_z]->ik_settings_.resize(2);
	modules_[e_arm_z]->ik_settings_[0] = { pos_to_pos, 0.4, targets[0], cnt_objects_[0] };		// 標的位置への位置IK
	modules_[e_arm_z]->ik_settings_[1] = { dirx_to_dirx, 0.01, targets[1], cnt_objects_[1] };	// 肘姿勢への姿勢IK
	// --- 5. e_wrist_z --- //
	modules_[e_wrist_z]->ik_settings_.clear();
	modules_[e_wrist_z]->ik_settings_.resize(2);
	modules_[e_wrist_z]->ik_settings_[0] = { dirz_to_pos, 0.4, targets[0], cnt_objects_[0] };
	modules_[e_wrist_z]->ik_settings_[1] = { dirx_to_dirx, 0.0, targets[0], cnt_objects_[0] };
	// --- 6. e_wrist_x --- //
	modules_[e_wrist_x]->ik_settings_.clear();
	modules_[e_wrist_x]->ik_settings_.resize(1);
	modules_[e_wrist_x]->ik_settings_[0] = { dirz_to_pos, 0.4, targets[0], cnt_objects_[0] };
	// --- 7. e_wrist_z2 --- //
	modules_[e_wrist_z2]->ik_settings_.clear();
	modules_[e_wrist_z2]->ik_settings_.resize(2);
	modules_[e_wrist_z2]->ik_settings_[0] = { dirz_to_pos, 0.0, targets[0], cnt_objects_[0] };
	modules_[e_wrist_z2]->ik_settings_[1] = { dirx_to_dirx, 0.1, targets[0], cnt_objects_[0] };

}
	

void Agn_armR001::mode01_update(float delta_time) {
	// ----- ターゲットに向かってエイム動作：IK更新 ----- //
	// ターゲットの数：2つ (照準位置、肘引きの目標位置)
	// targets[0] : 照準位置
	// targets[1] ： 肘引きの目標位置

	// ---- 制御対象の更新 ---- //
	cnt_objects_[0]->pos_ = modules_[e_wrist_z2]->pos_o_next_;	// 手先位置更新
	cnt_objects_[0]->Rotate(modules_[e_wrist_z2]->rot_tmp_);	// 手先姿勢更新
	cnt_objects_[0]->update(0);
	
	cnt_objects_[1]->pos_ = modules_[e_arm_z]->pos_o_;		// 肘位置更新
	cnt_objects_[1]->Rotate(modules_[e_arm_z]->rot_tmp_);		// 肘姿勢更新
	cnt_objects_[1]->update(0);

	tnl::Vector3 tmp_pos = pos_o_;
	tnl::Quaternion tmp_q = rot_tmp_;
	for (auto mod : modules_) {
		mod->DirectKinematicsWithIK_world(delta_time, tmp_pos, tmp_q);
		tmp_pos = mod->pos_o_next_;
		tmp_q = mod->rot_tmp_;
	}
	

	// 描画系アップデート
	update(delta_time);
}

void Agn_armR001::mode02_init(FaceVec& target) {

}

void Agn_armR001::mode02_update() {

}