#include "gm_agn_armR001.h"

Agn_armR001* Agn_armR001::Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- エージェントが管理するモジュールをインスタンス化＆参照リストに追加 ----- //
	Agn_armR001* agn = new Agn_armR001();
	agn->modules_.resize(e_modules_max);
	agn->rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	agn->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	agn->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	agn->link_length_ = 0;

	// ---- 1. e_sho_x ---- //
	Module* sho_x = new Module();
	sho_x->id_ = 1;
	sho_x->rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	sho_x->dir_z_ =	tnl::Vector3{ 0, 0, 1 };
	sho_x->dir_x_ =	tnl::Vector3{ 1, 0, 0 };
	sho_x->link_length_ = 0;
	sho_x->kp_p_nums_.push_back(0.3);
	sho_x->is_posIK = true;
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
	sho_y->rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	sho_y->dir_z_ =	tnl::Vector3{ 0, 0, 1 };
	sho_y->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	sho_y->link_length_ = 30;
	sho_y->kp_p_nums_.push_back(0.2);
	sho_y->is_posIK = true;
	Parts* sho_y_ln01 = new Parts();
	sho_y_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.0, 20);
	sho_y_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue_green.bmp"));
	sho_y_ln01->ofs_pos_ += sho_y->dir_z_ * sho_y->link_length_ / 2;
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
	arm_x->rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	arm_x->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	arm_x->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	arm_x->link_length_ = 0;
	arm_x->kp_p_nums_.push_back(0.2);
	arm_x->is_posIK = true;
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
	arm_z->rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	arm_z->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	arm_z->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	arm_z->link_length_ = 20;
	arm_z->kp_p_nums_.push_back(0.2);
	arm_z->is_posIK= true;
	Parts* arm_z_ln01 = new Parts();
	arm_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	arm_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red_blue.bmp"));
	arm_z_ln01->ofs_pos_ += arm_z->dir_z_ * arm_z->link_length_ / 2;
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
	wrist_z->rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_z->link_length_ = 10;
	wrist_z->kp_p_nums_.push_back(0.2);
	wrist_z->is_posIK = true;
	Parts* wrist_z_ln01 = new Parts();
	wrist_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 10);
	wrist_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z_ln01->ofs_pos_ += wrist_z->dir_z_ * wrist_z->link_length_ / 2;
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
	wrist_x->rot_axis_ = tnl::Vector3{ 1, 0, 0 };
	wrist_x->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_x->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_x->link_length_ = 0;
	wrist_x->kp_p_nums_.push_back(0.2);
	wrist_x->is_posIK = true;
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



	// ---- 7. e_wrist_y ---- //
	/*Module* wrist_y = new Module();
	wrist_y->rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	wrist_y->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_y->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_y->link_length_ = 10;
	wrist_y->kp_p_nums_.push_back(0.2);
	wrist_y->is_posIK = true;
	Parts* wrist_y_ln01 = new Parts();
	wrist_y_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.0, 10);
	wrist_y_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue_green.bmp"));
	wrist_y_ln01->ofs_pos_ += wrist_y->dir_z_ * wrist_y->link_length_ / 2;
	wrist_y_ln01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(-90));
	Parts* wrist_y_rot01 = new Parts();
	wrist_y_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 4);
	wrist_y_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	Parts* wrist_y_lid01 = new Parts();
	wrist_y_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_y_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	wrist_y_lid01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	wrist_y_lid01->ofs_pos_ += tnl::Vector3{ 0, 2.0, 0 };
	Parts* wrist_y_lid02 = new Parts();
	wrist_y_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_y_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	wrist_y_lid02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	wrist_y_lid02->ofs_pos_ += tnl::Vector3{ 0, -2.0, 0 };
	Parts* wrist_y_dirx01 = new Parts();
	wrist_y_dirx01->mesh_ = dxe::Mesh::CreateCylinder(1, 15);
	wrist_y_dirx01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/green.bmp"));
	wrist_y_dirx01->ofs_pos_ += tnl::Vector3{ 7.5, 0, 0 };
	wrist_y_dirx01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	wrist_y->parts_.push_back(wrist_y_ln01);
	wrist_y->parts_.push_back(wrist_y_rot01);
	wrist_y->parts_.push_back(wrist_y_lid01);
	wrist_y->parts_.push_back(wrist_y_lid02);
	wrist_y->parts_.push_back(wrist_y_dirx01);
	agn->modules_[e_wrist_y] = wrist_y;*/

	// ---- 7. e_wrist_z22 ---- //
	Module* wrist_z2 = new Module();
	wrist_z2->id_ = 7;
	wrist_z2->rot_axis_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z2->dir_z_ = tnl::Vector3{ 0, 0, 1 };
	wrist_z2->dir_x_ = tnl::Vector3{ 1, 0, 0 };
	wrist_z2->link_length_ = 10;
	wrist_z2->kp_p_nums_.push_back(0.2);
	wrist_z2->is_posIK = true;
	Parts* wrist_z2_ln01 = new Parts();
	wrist_z2_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 10);
	wrist_z2_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/red.bmp"));
	wrist_z2_ln01->ofs_pos_ += wrist_z2->dir_z_ * wrist_z2->link_length_ / 2;
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

	return agn;
}

void Agn_armR001::aimTarget_initialize(FaceVec& target, FaceVec& cnt_obj, FaceVec& elbow) {
	// ---- 肘を引いて、ターゲットを指さす動作を実行(initialize) ---- //
	// --- FaceVec参照値初期化 --- //
	for (auto mod : modules_) {
		mod->cnt_targets_.clear();
	}
	// --- 
	// --- 1. e_sho_x --- //
	// 位置参照 * 2
	modules_[e_sho_x]->cnt_targets_.push_back(&target);
	// --- 2. e_sho_z --- //
	// 位置参照 * 1
	modules_[e_sho_y]->cnt_targets_.push_back(&target);
	// --- 3. e_arm_x --- //
	// 位置参照 * 1
	modules_[e_arm_x]->cnt_targets_.push_back(&target);
	// --- 4. e_arm_y --- //
	// 姿勢x参照 * 1
	modules_[e_arm_z]->cnt_targets_.push_back(&target);
	// --- 5. e_wrist_z --- //
	// 姿勢x参照
	modules_[e_wrist_z]->cnt_targets_.push_back(&target);
	// --- 6. e_wrist_x --- //
	// 姿勢z参照
	modules_[e_wrist_x]->cnt_targets_.push_back(&target);
	// --- 7. e_wrist_y --- //
	// 姿勢z参照
	modules_[e_wrist_z2]->cnt_targets_.push_back(&target);

	// --- cnt_obj初期化 --- //
	cnt_obj.pos_ = modules_[e_wrist_z2]->pos_o_next_;
	cnt_obj.dir_z_ = modules_[e_wrist_z2]->dir_z_;
	cnt_obj.dir_x_ = modules_[e_wrist_z2]->dir_x_;
	cnt_obj.rot_sum_ = modules_[e_wrist_z2]->rot_sum_;
	cnt_obj.update(0);
	for (auto mod : modules_) {
		mod->cnt_objects_.clear();
	}
	// --- 1. e_sho_x --- //
	// 位置参照 * 2
	modules_[e_sho_x]->cnt_objects_.push_back(&cnt_obj);
	// --- 2. e_sho_z --- //
	// 位置参照 * 1
	modules_[e_sho_y]->cnt_objects_.push_back(&cnt_obj);
	// --- 3. e_arm_x --- //
	// 位置参照 * 1
	modules_[e_arm_x]->cnt_objects_.push_back(&cnt_obj);
	// --- 4. e_arm_y --- //
	// 姿勢x参照 * 1
	modules_[e_arm_z]->cnt_objects_.push_back(&cnt_obj);
	// --- 5. e_wrist_z --- //
	// 姿勢x参照
	modules_[e_wrist_z]->cnt_objects_.push_back(&cnt_obj);
	// --- 6. e_wrist_x --- //
	// 姿勢x参照
	modules_[e_wrist_x]->cnt_objects_.push_back(&cnt_obj);
	// --- 7. e_wrist_y --- //
	// 姿勢z参照
	modules_[e_wrist_z2]->cnt_objects_.push_back(&cnt_obj);
}

void Agn_armR001::aimTarget_update(float delta_time, const FaceVec& target) {
	// ---- 肘を引いて、ターゲットを指さす動作を実行(update) ---- //
	tnl::Vector3 tmp_p_back = this->pos_o_;
	tnl::Quaternion tmp_q_back = this->rot_tmp_;
	
	for (auto mod : modules_) {
		if (mod->id_ == 2) { printf(""); }
		mod->DirectKinematicsWithIK_world(delta_time, tmp_p_back, tmp_q_back);
		tmp_p_back = mod->pos_o_next_;
		tmp_q_back = mod->rot_tmp_;
	}
	cnt_objects_[0]->pos_ = modules_[e_wrist_z2]->pos_o_next_;
	cnt_objects_[0]->dir_z_ = modules_[e_wrist_z2]->dir_z_;
	cnt_objects_[0]->dir_x_ = modules_[e_wrist_z2]->dir_x_;
	cnt_objects_[0]->rot_sum_ = modules_[e_wrist_z2]->rot_sum_;
	cnt_objects_[0]->update(0);

}