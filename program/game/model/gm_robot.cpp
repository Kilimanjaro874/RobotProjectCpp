#include "gm_robot.h"
#include "gm_face_vector.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //


	// ---- 0. Robot[id = 0] : パラメータ初期化 ---- //
	Robot* rob = new Robot();
	rob->InitParams(1, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK パラメータ初期化
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 7, 0}, 1, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
	// モジュールサイズ設定
	rob->modules_.resize(e_modules_max);

	// ---- 1. lower back : パラメータ初期化---- //
	rob->modules_[e_lower_back_].resize(3);		// 3DOF
	// --- 1.1. e_bb_y --- //
	Module* bb_y = new Module();
	// 子設定
	rob->next.resize(1);
	rob->next[0] = bb_y;
	// 親設定 
	bb_y->back = rob;
	// パラメータ設定
	bb_y->InitParams(100, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK パラメータ初期化
	bb_y->dk_s_v_.resize(1);
	bb_y->dk_s_v_[0] = { 101, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bb_y->in_rot_axis_, 0) };

	// --- 1.2. e_bb_x --- //
	Module* bb_x = new Module();
	// 子設定
	bb_y->next.resize(1);
	bb_y->next[0] = bb_x;
	// 親設定
	bb_x->back = bb_y;
	// パラメータ設定
	bb_x->InitParams(101, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	// DK パラメータ初期化
	bb_x->dk_s_v_.resize(1);
	bb_x->dk_s_v_[0] = { 102, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bb_x->in_rot_axis_, 0) };

	// --- 1.3. e_bb_z --- //
	Module* bb_z = new Module();
	// 子設定
	bb_x->next.resize(1);
	bb_x->next[0] = bb_z;
	// 親設定
	bb_z->back = bb_x;
	// パラメータ設定
	bb_z->InitParams(102, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	// DK パラメータ初期化
	bb_z->dk_s_v_.resize(4);	// body, r_leg, l_leg, object用
	bb_z->dk_s_v_[0] = { 200, {0, 1, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };		// for e_body
	bb_z->dk_s_v_[1] = { 300, {-1.5, -1, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_ r_leg
	bb_z->dk_s_v_[2] = { 400, {1.5, -1, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_l_leg
	bb_z->dk_s_v_[3] = { 800, {0, 1, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_bb_ object
	// Parts
	Parts* bb_z_bone = new Parts();
	bb_z_bone->mesh_ = dxe::Mesh::CreateCylinder(0.3, 2);
	bb_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bb_z->parts_.push_back(bb_z_bone);
	Parts* bb_z_pelvis = new Parts();
	bb_z_pelvis->mesh_ = dxe::Mesh::CreateCylinder(0.3, 3);
	bb_z_pelvis->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bb_z_pelvis->ofs_pos_ += {0, -1, 0};
	bb_z_pelvis->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	bb_z->parts_.push_back(bb_z_pelvis);


	// ---- 2. body : パラメータ初期化 ---- //
	rob->modules_[e_body_].resize(3);

	// --- 2.1. e_bo_y --- //
	Module* bo_y = new Module();
	// 子設定
	bb_z->next.resize(1);
	bb_z->next[0] = bo_y;
	// 親設定
	bo_y->back = bb_z;
	// パラメータ設定
	bo_y->InitParams(200, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK パラメータ初期化
	bo_y->dk_s_v_.resize(1);
	bo_y->dk_s_v_[0] = { 201, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bo_y->in_rot_axis_, 0) };

	// --- 2.2. e_bo_x --- //
	Module* bo_x = new Module();
	// 子設定
	bo_y->next.resize(1);
	bo_y->next[0] = bo_x;
	// 親設定
	bo_x->back = bo_y;
	// パラメータ設定
	bo_x->InitParams(201, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	// DK パラメータ初期化
	bo_x->dk_s_v_.resize(1);
	bo_x->dk_s_v_[0] = { 202, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bo_x->in_rot_axis_, 0) };

	// --- 2. 3. e_bo_z --- //
	Module* bo_z = new Module();
	// 子設定
	bo_x->next.resize(1);
	bo_x->next[0] = bo_z;
	// 親設定
	bo_z->back = bo_x;
	// パラメータ初期化
	bo_z->InitParams(202, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	bo_z->dk_s_v_.resize(4);
	bo_z->dk_s_v_[0] = { 500, {0, 2, 0}, 1, tnl::Quaternion::RotationAxis(bo_z->in_rot_axis_, 0) };	// for e_head
	bo_z->dk_s_v_[1] = { 600, {-2, 2, 0}, 1, tnl::Quaternion::RotationAxis(bo_z->in_rot_axis_, 0) };	// for e_r_arm
	bo_z->dk_s_v_[2] = { 700, {2, 2, 0}, 1, tnl::Quaternion::RotationAxis(bo_z->in_rot_axis_, 0) }; // for e_l_arm
	bo_z->dk_s_v_[3] = { 820, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bo_z->in_rot_axis_, 0) };	// for object
	// Parts
	Parts* bo_z_bone = new Parts();
	bo_z_bone->mesh_ = dxe::Mesh::CreateCylinder(0.4, 0.5);
	bo_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z_bone->ofs_pos_ += {0, 0.25, 0};
	bo_z->parts_.push_back(bo_z_bone);
	Parts* bo_z_sterR = new Parts();
	bo_z_sterR->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
	bo_z_sterR->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z_sterR->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(45));
	bo_z_sterR->ofs_pos_ += {-0.75, 1.25, 0};
	bo_z->parts_.push_back(bo_z_sterR);
	Parts* bo_z_sterL = new Parts();
	bo_z_sterL->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
	bo_z_sterL->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z_sterL->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(-45));
	bo_z_sterL->ofs_pos_ += {0.75, 1.25, 0};
	bo_z->parts_.push_back(bo_z_sterL);
	Parts* bo_z_shoulder = new Parts();
	bo_z_shoulder->mesh_ = dxe::Mesh::CreateCylinder(0.2, 4.0);
	bo_z_shoulder->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z_shoulder->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	bo_z_shoulder->ofs_pos_ += {0, 2, 0};
	bo_z->parts_.push_back(bo_z_shoulder);
	Parts* bo_z_core = new Parts();
	bo_z_core->mesh_ = dxe::Mesh::CreateSphere(0.75);
	bo_z_core->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z_core->ofs_pos_ += {0, 1.2, 0};
	bo_z->parts_.push_back(bo_z_core);

	// ---- 3. r_Leg ： パラメータ初期化 ---- //
	// --- 3.1. e_r_leg_x --- //
	Module* leg_x_r = new Module();
	// 子設定
	bb_z->next.push_back(leg_x_r);
	// 親設定
	leg_x_r->back = bb_z;
	// パラメータ設定
	leg_x_r->InitParams(300, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DKパラメータ初期化
	leg_x_r->dk_s_v_.resize(1);
	leg_x_r->dk_s_v_[0] = { 301, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_x_r->in_rot_axis_, 0) };

	// --- 3.2. e_r_leg_z --- //
	Module* leg_z_r = new Module();
	// 子設定
	leg_x_r->next.resize(1);
	leg_x_r->next[0] = leg_z_r;
	// 親設定
	leg_z_r->back = leg_x_r;
	// パラメータ設定
	leg_z_r->InitParams(301, { 0, 0, -1 }, tnl::Quaternion::RotationAxis({ 0, 0, -1 }, 0));
	// DKパラメータ初期化
	leg_z_r->dk_s_v_.resize(1);
	leg_z_r->dk_s_v_[0] = { 302, {0, -2.5, 0}, 1, tnl::Quaternion::RotationAxis(leg_x_r->in_rot_axis_, 0) };
	// Parts
	Parts* leg_z_r_hj1 = new Parts();
	leg_z_r_hj1->mesh_ = dxe::Mesh::CreateSphere(0.4);
	leg_z_r_hj1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z_r->parts_.push_back(leg_z_r_hj1);
	Parts* leg_z_r_th1 = new Parts();
	leg_z_r_th1->mesh_ = dxe::Mesh::CreateCylinder(0.3, 2.5);
	leg_z_r_th1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z_r_th1->ofs_pos_ += {0, -1.25, 0};
	leg_z_r->parts_.push_back(leg_z_r_th1);

	// --- 3.3. e_r_leg_x2 --- //
	Module* leg_x2_r = new Module();
	// 子設定
	leg_z_r->next.resize(1);
	leg_z_r->next[0] = leg_x2_r;
	// 親設定
	leg_x2_r->back = leg_z_r;
	// パラメータ設定
	leg_x2_r->InitParams(302, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DKパラメータ初期化
	leg_x2_r->dk_s_v_.resize(1);
	leg_x2_r->dk_s_v_[0] = { 303, {0, -2.5, 0}, 1, tnl::Quaternion::RotationAxis(leg_x2_r->in_rot_axis_, 0) };
	// Parts
	Parts* leg_x2_r_j = new Parts();
	leg_x2_r_j->mesh_ = dxe::Mesh::CreateSphere(0.4);
	leg_x2_r_j->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_x2_r->parts_.push_back(leg_x2_r_j);

	Parts* leg_x2_r_th2 = new Parts();
	leg_x2_r_th2->mesh_ = dxe::Mesh::CreateCylinder(0.3, 2.5);
	leg_x2_r_th2->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_x2_r_th2->ofs_pos_ += {0, -1.25, 0};
	leg_x2_r->parts_.push_back(leg_x2_r_th2);

	// --- 3.4. e_r_leg_x3 --- //
	Module* leg_x3_r = new Module();
	// 子設定
	leg_x2_r->next.resize(1);
	leg_x2_r->next[0] = leg_x3_r;
	// 親設定
	leg_x3_r->back = leg_x2_r;
	// パラメータ設定
	leg_x3_r->InitParams(303, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DKパラメータ初期化
	leg_x3_r->dk_s_v_.resize(1);
	leg_x3_r->dk_s_v_[0] = { 304, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_x3_r->in_rot_axis_, 0) };

	// --- 3.5. e_r_leg_y --- //
	Module* leg_y_r = new Module();
	// 子設定
	leg_x3_r->next.resize(1);
	leg_x3_r->next[0] = leg_y_r;
	// 親設定
	leg_y_r->back = leg_x3_r;
	// パラメータ設定 
	leg_y_r->InitParams(304, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	// DKパラメータ初期化
	leg_y_r->dk_s_v_.resize(1);
	leg_y_r->dk_s_v_[0] = { 305, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_y_r->in_rot_axis_, 0) };

	// --- 3.6. e_r_leg_z2 --- //
	Module* leg_z2_r = new Module();
	// 子設定
	leg_y_r->next.resize(1);
	leg_y_r->next[0] = leg_z2_r;
	// 親設定
	leg_z2_r->back = leg_y_r;
	// パラメータ設定
	leg_z2_r->InitParams(305, { 0, 0, -1 }, tnl::Quaternion::RotationAxis({ 0, 0, -1 }, 0));
	// DKパラメータ初期化
	leg_z2_r->dk_s_v_.resize(1);
	leg_z2_r->dk_s_v_[0] = { 800, {0, -1, 0}, 1, tnl::Quaternion::RotationAxis(leg_z2_r->in_rot_axis_, 0) };
	// Parts 
	Parts* leg_z2_r_j1 = new Parts();
	leg_z2_r_j1->mesh_ = dxe::Mesh::CreateSphere(0.4);
	leg_z2_r_j1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z2_r->parts_.push_back(leg_z2_r_j1);
	Parts* leg_z2_r_th3 = new Parts();
	leg_z2_r_th3->mesh_ = dxe::Mesh::CreateCylinder(0.3, 1.0);
	leg_z2_r_th3->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z2_r_th3->ofs_pos_ += {0, -0.5, 0};
	leg_z2_r->parts_.push_back(leg_z2_r_th3);
	Parts* leg_z2_r_ft = new Parts();
	leg_z2_r_ft->mesh_ = dxe::Mesh::CreateBox(0.5);
	leg_z2_r_ft->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z2_r_ft->ofs_pos_ += {0, -0.75, 0};
	leg_z2_r->parts_.push_back(leg_z2_r_ft);
	Parts* leg_z2_r_toe = new Parts();
	leg_z2_r_toe->mesh_ = dxe::Mesh::CreateCylinder(0.4, 1.0);
	leg_z2_r_toe->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	leg_z2_r_toe->ofs_pos_ += {0, -0.75, -0.5};
	leg_z2_r_toe->ofs_rot_ = tnl::Quaternion::RotationAxis({ -1, 0, 0 }, tnl::ToRadian(90));
	leg_z2_r->parts_.push_back(leg_z2_r_toe);

	// ---- 4. l_Leg ---- //
	
	// ---- 5. head ---- //
	// --- 5.1. e_head_x --- //
	Module* head_x = new Module();
	// 子設定
	bo_z->next.push_back(head_x);
	// 親設定
	head_x->back = bo_z;
	// パラメータ初期化
	head_x->InitParams(500, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	head_x->dk_s_v_.resize(1);
	head_x->dk_s_v_[0] = { 501, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(head_x->in_rot_axis_, 0) };

	// --- 5.2. e_head_y --- //
	Module* head_y = new Module();
	// 子設定
	head_x->next.push_back(head_y);
	// 親設定
	head_y->back = head_x;
	// パラメータ初期化
	head_y->InitParams(501, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	head_y->dk_s_v_.resize(1);
	head_y->dk_s_v_[0] = { 502, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(head_y->in_rot_axis_, 0) };
	
	// --- 5.3. e_head_z --- //
	Module* head_z = new Module();
	// 子設定
	head_y->next.push_back(head_z);
	// 親設定
	head_z->back = head_y;
	// パラメータ初期化
	head_z->InitParams(502, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	head_z->dk_s_v_.resize(1);
	head_z->dk_s_v_[0] = { 850, {0, 0.5, 0}, 1, tnl::Quaternion::RotationAxis(head_z->in_rot_axis_, 0) };
	Parts* head_neck = new Parts();
	head_neck->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
	head_neck->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	head_z->parts_.push_back(head_neck);
	Parts* head_head = new Parts();
	head_head->mesh_ = dxe::Mesh::CreateSphere(0.4);
	head_head->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	head_head->ofs_pos_ += {0, 0.5, 0};
	head_z->parts_.push_back(head_head);


	// ---- 6. right Arm ---- //
	rob->modules_[e_r_arm].resize(7);

	// --- 6.1. e_sho_x_r --- //
	Module* sho_x_r = new Module();
	// 子設定
	bo_z->next.push_back (sho_x_r);
	// 親設定
	sho_x_r->back = bo_z;
	// パラメータ初期化
	sho_x_r->InitParams(600, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 },0));
	sho_x_r->dk_s_v_.resize(1);
	sho_x_r->dk_s_v_[0] = { 601, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(sho_x_r->in_rot_axis_, 0) };

	// --- 6.2. e_sho_z_r --- //
	Module* sho_z_r = new Module();
	// 子設定
	sho_x_r->next.resize(1);
	sho_x_r->next[0] = sho_z_r;
	// 親設定
	sho_z_r->back = sho_x_r;
	// パラメータ初期化
	sho_z_r->InitParams(601, { 0, 0, -1 }, tnl::Quaternion::RotationAxis({ 0, 0, -1 }, 0));
	sho_z_r->dk_s_v_.resize(1);
	sho_z_r->dk_s_v_[0] = { 602, {0, -2, 0}, 1, tnl::Quaternion::RotationAxis(sho_z_r->in_rot_axis_, 0) };
	// Parts
	Parts* sho_z_r_jnt1 = new Parts();
	sho_z_r_jnt1->mesh_ = dxe::Mesh::CreateSphere(0.3);
	sho_z_r_jnt1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_r->parts_.push_back(sho_z_r_jnt1);
	Parts* sho_z_r_arm1 = new Parts();
	sho_z_r_arm1->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.0);
	sho_z_r_arm1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_r_arm1->ofs_pos_ += {0, -1, 0};
	sho_z_r->parts_.push_back(sho_z_r_arm1);

	// --- 6.3. e_arm_x --- //
	Module* arm_x_r = new Module();
	// 子設定
	sho_z_r->next.resize(1);
	sho_z_r->next[0] = arm_x_r;
	// 親設定
	arm_x_r->back = sho_z_r;
	// パラメータ初期化
	arm_x_r->InitParams(602, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	arm_x_r->dk_s_v_.resize(1);
	arm_x_r->dk_s_v_[0] = { 603, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(arm_x_r->in_rot_axis_, 0) };

	// --- 6.4. e_arm_y --- //
	Module* arm_y_r = new Module();
	// 子設定
	arm_x_r->next.resize(1);
	arm_x_r->next[0] = arm_y_r;
	// 親設定
	arm_y_r->back = arm_x_r;
	// パラメータ初期化
	arm_y_r->InitParams(603, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	arm_y_r->dk_s_v_.resize(1);
	arm_y_r->dk_s_v_[0] = { 604, {0, -2, 0}, 1, tnl::Quaternion::RotationAxis(arm_y_r->in_rot_axis_, 0) };
	// Parts
	Parts* arm_y_r_jnt2 = new Parts();
	arm_y_r_jnt2->mesh_ = dxe::Mesh::CreateSphere(0.3);
	arm_y_r_jnt2->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_r->parts_.push_back(arm_y_r_jnt2);
	Parts* arm_y_r_arm2 = new Parts();
	arm_y_r_arm2->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.0);
	arm_y_r_arm2->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_r_arm2->ofs_pos_ += {0, -1, 0};
	arm_y_r->parts_.push_back(arm_y_r_arm2);

	// --- 6.5. e_wrist_y --- //
	Module* wri_y_r = new Module();
	// 子設定
	arm_y_r->next.resize(1);
	arm_y_r->next[0] = wri_y_r;
	// 親設定
	wri_y_r->back = arm_y_r;
	// パラメータ初期化
	wri_y_r->InitParams(604, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	wri_y_r->dk_s_v_.resize(1);
	wri_y_r->dk_s_v_[0] = { 605, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(wri_y_r->in_rot_axis_, 0) };

	// --- 6.6. e_wrist_x
	Module* wri_x_r = new Module();
	// 子設定
	wri_y_r->next.resize(1);
	wri_y_r->next[0] = wri_x_r;
	// 親設定
	wri_x_r->back = wri_y_r;
	// パラメータ初期化
	wri_x_r->InitParams(605, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	wri_x_r->dk_s_v_.resize(1);
	wri_x_r->dk_s_v_[0] = { 606, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(wri_x_r->in_rot_axis_, 0) };

	// --- 6.7. e_wrist_y --- //
	Module* wri_y2_r = new Module();
	// 子設定
	wri_x_r->next.resize(1);
	wri_x_r->next[0] = wri_y2_r;
	// 親設定
	wri_y2_r->back = wri_x_r;
	// パラメータ初期化	
	wri_y2_r->InitParams(606, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	wri_y2_r->dk_s_v_.resize(2);
	wri_y2_r->dk_s_v_[0] = { 860, {0, -0.75, 0}, 1, tnl::Quaternion::RotationAxis(wri_y2_r->in_rot_axis_, 0) };
	wri_y2_r->dk_s_v_[1] = { 960, {0, -0.75, 0}, 1, tnl::Quaternion::RotationAxis(wri_y2_r->in_rot_axis_, 0) };
	// Parts
	Parts* wri_y2_r_jnt3 = new Parts();
	wri_y2_r_jnt3->mesh_ = dxe::Mesh::CreateSphere(0.3);
	wri_y2_r_jnt3->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wri_y2_r->parts_.push_back(wri_y2_r_jnt3);
	Parts* wri_y2_r_wrist = new Parts();
	wri_y2_r_wrist->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.75);
	wri_y2_r_wrist->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wri_y2_r_wrist->ofs_pos_ += {0, -0.75/2, 0};
	wri_y2_r->parts_.push_back(wri_y2_r_wrist);


	// ----- ロボ全てのパーツの初期位置・姿勢を反映 ----- //
	rob->rob_dk_s_v_.resize(1);
	tnl::Vector3 tmp_pos_dir = pos;
	tmp_pos_dir.normalize();
	float pos_length = pos.length();
	rob->rob_dk_s_v_[0] = {1, tmp_pos_dir, pos_length, rot};
	rob->InitDK(rob->rob_dk_s_v_);
	rob->AllInitDK(rob, rob->dk_s_v_);
	rob->updateTree(rob, 0);

	return rob;
}

void Robot::mode01_init(float delta_time) {
	// ----- 射撃モード初期化 ----- //

	// ---- Test ---- //
	// 右腕のIKを実施する
	objects_.clear();
	objects_.resize(e_modules_max);
	objects_[e_r_arm].push_back(FaceVector::Create(0, { 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)));
	objects_[e_r_arm][0]->InitParams(860, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	// 右腕手先にオブジェクト取付
	AttachModule(this, objects_[e_r_arm][0], 606);

	// 右腕手先の目標位置・姿勢を登録
	targets_.clear();
	targets_.resize(e_modules_max);
	targets_[e_r_arm].push_back(FaceVector::Create(0, { -2, 8, -2 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)));

	// 右腕のIK動作試験のための仕込み
	Module* t = targets_[e_r_arm][0];
	Module* o = objects_[e_r_arm][0];
	SetIKParams(this, 600, t, o);	
	SetIKParams(this, 601, t, o);
	SetIKParams(this, 602, t, o);
	SetIKParams(this, 603, t, o);
	SetIKParams(this, 604, t, o);
	SetIKParams(this, 605, t, o);
	SetIKParams(this, 606, t, o);

}

void Robot::mode01_update(float delta_time) {

	// ---- Test ---- //
	move(delta_time);				// ユーザー入力受付
	moveDK();						// 入力内容に応じてDK実施
	AllupdateIK(this, delta_time);	// 全てのモジュールのDK&IK実施
	updateTree(this, delta_time);	// 全てののモジュールの部品位置・姿勢アップデート

	targets_[e_r_arm][0]->update(delta_time);
	// ---- ターゲットのIKを実施 ---- //

	
}

void Robot::move(float delta_time) {
	// ----- ユーザー入力で移動[座標平行移動＆姿勢変更]を行う ----- //
	// ---- 初期化 ---- //
	rot_move_ = tnl::Quaternion::RotationAxis(in_rot_axis_, 0);
	d_move_ = tnl::Vector3{ 0, 0, 0 };
	// ---- 入力受付 ---- //
	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		d_move_ += dir_z_ * delta_time * move_speed_;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		d_move_ += dir_z_ * delta_time * move_speed_ * -1;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		rot_move_ = tnl::Quaternion::RotationAxis(in_rot_axis_, rotate_speed_ * delta_time);
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		rot_move_ = tnl::Quaternion::RotationAxis(in_rot_axis_, -rotate_speed_ * delta_time);
	}

}

void Robot::moveDK() {
	// ---- ユーザー入力による位置・姿勢変化を本ロボットクラスに反映 ---- //
	rot_sum_ *= rot_move_;
	pos_o_ += d_move_;
	dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);

	// ---- ユーザー入力の位置・姿勢変化情報をdk_settingに格納
	for (int i = 0; i < dk_s_v_.size(); i++) {
		dk_s_v_[i].dir_r_n_ = pos_o_ +
			tnl::Vector3::TransformCoord(in_dk_s_v_[i].dir_r_n_, rot_sum_) * in_dk_s_v_[i].dir_r_length_;
		dk_s_v_[i].q_r_n_ = rot_move_;
	}
}

void Robot::targetInit() {
	// ---- ロボットが所持しているtargetクラスの初期化を実施 ---- //


}

void Robot::targetDK() {
	// ---- ロボットが所持しているtargetクラスを本座標系周りにDK計算する ---- //
	for (int n = 0; n < targets_.size(); n++) {
		for (int m = 0; m < targets_[n].size(); m++) {
			targets_[n][m]->rot_sum_ *= rot_move_;
		}
	}

}











