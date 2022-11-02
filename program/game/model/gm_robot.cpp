#include "gm_robot.h"
#include "gm_face_vector.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- �ʒu pos, �p�� rot �փ��{�b�g�𐶐� ----- //


	// ---- 0. Robot[id = 0] : �p�����[�^������ ---- //
	Robot* rob = new Robot();
	rob->InitParams(1, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK �p�����[�^������
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 7, 0}, 1, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
	// ���W���[���T�C�Y�ݒ�
	rob->modules_.resize(e_modules_max);

	// ---- 1. lower back : �p�����[�^������---- //
	rob->modules_[e_lower_back_].resize(3);		// 3DOF
	// --- 1.1. e_bb_y --- //
	Module* bb_y = new Module();
	// �q�ݒ�
	rob->next.resize(1);
	rob->next[0] = bb_y;
	// �e�ݒ� 
	bb_y->back = rob;
	// �p�����[�^�ݒ�
	bb_y->InitParams(100, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK �p�����[�^������
	bb_y->dk_s_v_.resize(1);
	bb_y->dk_s_v_[0] = { 101, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bb_y->in_rot_axis_, 0) };

	// --- 1.2. e_bb_x --- //
	Module* bb_x = new Module();
	// �q�ݒ�
	bb_y->next.resize(1);
	bb_y->next[0] = bb_x;
	// �e�ݒ�
	bb_x->back = bb_y;
	// �p�����[�^�ݒ�
	bb_x->InitParams(101, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	// DK �p�����[�^������
	bb_x->dk_s_v_.resize(1);
	bb_x->dk_s_v_[0] = { 102, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bb_x->in_rot_axis_, 0) };

	// --- 1.3. e_bb_z --- //
	Module* bb_z = new Module();
	// �q�ݒ�
	bb_x->next.resize(1);
	bb_x->next[0] = bb_z;
	// �e�ݒ�
	bb_z->back = bb_x;
	// �p�����[�^�ݒ�
	bb_z->InitParams(102, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	// DK �p�����[�^������
	bb_z->dk_s_v_.resize(4);	// body, r_leg, l_leg, object�p
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


	// ---- 2. body : �p�����[�^������ ---- //
	rob->modules_[e_body_].resize(3);

	// --- 2.1. e_bo_y --- //
	Module* bo_y = new Module();
	// �q�ݒ�
	bb_z->next.resize(1);
	bb_z->next[0] = bo_y;
	// �e�ݒ�
	bo_y->back = bb_z;
	// �p�����[�^�ݒ�
	bo_y->InitParams(200, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// DK �p�����[�^������
	bo_y->dk_s_v_.resize(1);
	bo_y->dk_s_v_[0] = { 201, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bo_y->in_rot_axis_, 0) };

	// --- 2.2. e_bo_x --- //
	Module* bo_x = new Module();
	// �q�ݒ�
	bo_y->next.resize(1);
	bo_y->next[0] = bo_x;
	// �e�ݒ�
	bo_x->back = bo_y;
	// �p�����[�^�ݒ�
	bo_x->InitParams(201, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	// DK �p�����[�^������
	bo_x->dk_s_v_.resize(1);
	bo_x->dk_s_v_[0] = { 202, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(bo_x->in_rot_axis_, 0) };

	// --- 2. 3. e_bo_z --- //
	Module* bo_z = new Module();
	// �q�ݒ�
	bo_x->next.resize(1);
	bo_x->next[0] = bo_z;
	// �e�ݒ�
	bo_z->back = bo_x;
	// �p�����[�^������
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

	// ---- 3. r_Leg �F �p�����[�^������ ---- //
	// --- 3.1. e_r_leg_x --- //
	Module* leg_x_r = new Module();
	// �q�ݒ�
	bb_z->next.push_back(leg_x_r);
	// �e�ݒ�
	leg_x_r->back = bb_z;
	// �p�����[�^�ݒ�
	leg_x_r->InitParams(300, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DK�p�����[�^������
	leg_x_r->dk_s_v_.resize(1);
	leg_x_r->dk_s_v_[0] = { 301, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_x_r->in_rot_axis_, 0) };

	// --- 3.2. e_r_leg_z --- //
	Module* leg_z_r = new Module();
	// �q�ݒ�
	leg_x_r->next.resize(1);
	leg_x_r->next[0] = leg_z_r;
	// �e�ݒ�
	leg_z_r->back = leg_x_r;
	// �p�����[�^�ݒ�
	leg_z_r->InitParams(301, { 0, 0, -1 }, tnl::Quaternion::RotationAxis({ 0, 0, -1 }, 0));
	// DK�p�����[�^������
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
	// �q�ݒ�
	leg_z_r->next.resize(1);
	leg_z_r->next[0] = leg_x2_r;
	// �e�ݒ�
	leg_x2_r->back = leg_z_r;
	// �p�����[�^�ݒ�
	leg_x2_r->InitParams(302, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DK�p�����[�^������
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
	// �q�ݒ�
	leg_x2_r->next.resize(1);
	leg_x2_r->next[0] = leg_x3_r;
	// �e�ݒ�
	leg_x3_r->back = leg_x2_r;
	// �p�����[�^�ݒ�
	leg_x3_r->InitParams(303, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	// DK�p�����[�^������
	leg_x3_r->dk_s_v_.resize(1);
	leg_x3_r->dk_s_v_[0] = { 304, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_x3_r->in_rot_axis_, 0) };

	// --- 3.5. e_r_leg_y --- //
	Module* leg_y_r = new Module();
	// �q�ݒ�
	leg_x3_r->next.resize(1);
	leg_x3_r->next[0] = leg_y_r;
	// �e�ݒ�
	leg_y_r->back = leg_x3_r;
	// �p�����[�^�ݒ� 
	leg_y_r->InitParams(304, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	// DK�p�����[�^������
	leg_y_r->dk_s_v_.resize(1);
	leg_y_r->dk_s_v_[0] = { 305, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(leg_y_r->in_rot_axis_, 0) };

	// --- 3.6. e_r_leg_z2 --- //
	Module* leg_z2_r = new Module();
	// �q�ݒ�
	leg_y_r->next.resize(1);
	leg_y_r->next[0] = leg_z2_r;
	// �e�ݒ�
	leg_z2_r->back = leg_y_r;
	// �p�����[�^�ݒ�
	leg_z2_r->InitParams(305, { 0, 0, -1 }, tnl::Quaternion::RotationAxis({ 0, 0, -1 }, 0));
	// DK�p�����[�^������
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
	// �q�ݒ�
	bo_z->next.push_back(head_x);
	// �e�ݒ�
	head_x->back = bo_z;
	// �p�����[�^������
	head_x->InitParams(500, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	head_x->dk_s_v_.resize(1);
	head_x->dk_s_v_[0] = { 501, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(head_x->in_rot_axis_, 0) };

	// --- 5.2. e_head_y --- //
	Module* head_y = new Module();
	// �q�ݒ�
	head_x->next.push_back(head_y);
	// �e�ݒ�
	head_y->back = head_x;
	// �p�����[�^������
	head_y->InitParams(501, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	head_y->dk_s_v_.resize(1);
	head_y->dk_s_v_[0] = { 502, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(head_y->in_rot_axis_, 0) };
	
	// --- 5.3. e_head_z --- //
	Module* head_z = new Module();
	// �q�ݒ�
	head_y->next.push_back(head_z);
	// �e�ݒ�
	head_z->back = head_y;
	// �p�����[�^������
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
	// �q�ݒ�
	bo_z->next.push_back (sho_x_r);
	// �e�ݒ�
	sho_x_r->back = bo_z;
	// �p�����[�^������
	sho_x_r->InitParams(600, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 },0));
	sho_x_r->dk_s_v_.resize(1);
	sho_x_r->dk_s_v_[0] = { 601, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(sho_x_r->in_rot_axis_, 0) };

	// --- 6.2. e_sho_z_r --- //
	Module* sho_z_r = new Module();
	// �q�ݒ�
	sho_x_r->next.resize(1);
	sho_x_r->next[0] = sho_z_r;
	// �e�ݒ�
	sho_z_r->back = sho_x_r;
	// �p�����[�^������
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
	// �q�ݒ�
	sho_z_r->next.resize(1);
	sho_z_r->next[0] = arm_x_r;
	// �e�ݒ�
	arm_x_r->back = sho_z_r;
	// �p�����[�^������
	arm_x_r->InitParams(602, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	arm_x_r->dk_s_v_.resize(1);
	arm_x_r->dk_s_v_[0] = { 603, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(arm_x_r->in_rot_axis_, 0) };

	// --- 6.4. e_arm_y --- //
	Module* arm_y_r = new Module();
	// �q�ݒ�
	arm_x_r->next.resize(1);
	arm_x_r->next[0] = arm_y_r;
	// �e�ݒ�
	arm_y_r->back = arm_x_r;
	// �p�����[�^������
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
	// �q�ݒ�
	arm_y_r->next.resize(1);
	arm_y_r->next[0] = wri_y_r;
	// �e�ݒ�
	wri_y_r->back = arm_y_r;
	// �p�����[�^������
	wri_y_r->InitParams(604, { 0, -1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	wri_y_r->dk_s_v_.resize(1);
	wri_y_r->dk_s_v_[0] = { 605, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(wri_y_r->in_rot_axis_, 0) };

	// --- 6.6. e_wrist_x
	Module* wri_x_r = new Module();
	// �q�ݒ�
	wri_y_r->next.resize(1);
	wri_y_r->next[0] = wri_x_r;
	// �e�ݒ�
	wri_x_r->back = wri_y_r;
	// �p�����[�^������
	wri_x_r->InitParams(605, { -1, 0, 0 }, tnl::Quaternion::RotationAxis({ -1, 0, 0 }, 0));
	wri_x_r->dk_s_v_.resize(1);
	wri_x_r->dk_s_v_[0] = { 606, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(wri_x_r->in_rot_axis_, 0) };

	// --- 6.7. e_wrist_y --- //
	Module* wri_y2_r = new Module();
	// �q�ݒ�
	wri_x_r->next.resize(1);
	wri_x_r->next[0] = wri_y2_r;
	// �e�ݒ�
	wri_y2_r->back = wri_x_r;
	// �p�����[�^������	
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


	// ----- ���{�S�Ẵp�[�c�̏����ʒu�E�p���𔽉f ----- //
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
	// ----- �ˌ����[�h������ ----- //

	// ---- Test ---- //
	// �E�r��IK�����{����
	objects_.clear();
	objects_.resize(e_modules_max);
	objects_[e_r_arm].push_back(FaceVector::Create(0, { 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)));
	objects_[e_r_arm][0]->InitParams(860, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, -1, 0 }, 0));
	// �E�r���ɃI�u�W�F�N�g��t
	AttachModule(this, objects_[e_r_arm][0], 606);

	// �E�r���̖ڕW�ʒu�E�p����o�^
	targets_.clear();
	targets_.resize(e_modules_max);
	targets_[e_r_arm].push_back(FaceVector::Create(0, { -2, 8, -2 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)));

	// �E�r��IK���쎎���̂��߂̎d����
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
	move(delta_time);				// ���[�U�[���͎�t
	moveDK();						// ���͓��e�ɉ�����DK���{
	AllupdateIK(this, delta_time);	// �S�Ẵ��W���[����DK&IK���{
	updateTree(this, delta_time);	// �S�Ă̂̃��W���[���̕��i�ʒu�E�p���A�b�v�f�[�g

	targets_[e_r_arm][0]->update(delta_time);
	// ---- �^�[�Q�b�g��IK�����{ ---- //

	
}

void Robot::move(float delta_time) {
	// ----- ���[�U�[���͂ňړ�[���W���s�ړ����p���ύX]���s�� ----- //
	// ---- ������ ---- //
	rot_move_ = tnl::Quaternion::RotationAxis(in_rot_axis_, 0);
	d_move_ = tnl::Vector3{ 0, 0, 0 };
	// ---- ���͎�t ---- //
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
	// ---- ���[�U�[���͂ɂ��ʒu�E�p���ω���{���{�b�g�N���X�ɔ��f ---- //
	rot_sum_ *= rot_move_;
	pos_o_ += d_move_;
	dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);

	// ---- ���[�U�[���͂̈ʒu�E�p���ω�����dk_setting�Ɋi�[
	for (int i = 0; i < dk_s_v_.size(); i++) {
		dk_s_v_[i].dir_r_n_ = pos_o_ +
			tnl::Vector3::TransformCoord(in_dk_s_v_[i].dir_r_n_, rot_sum_) * in_dk_s_v_[i].dir_r_length_;
		dk_s_v_[i].q_r_n_ = rot_move_;
	}
}

void Robot::targetInit() {
	// ---- ���{�b�g���������Ă���target�N���X�̏����������{ ---- //


}

void Robot::targetDK() {
	// ---- ���{�b�g���������Ă���target�N���X��{���W�n�����DK�v�Z���� ---- //
	for (int n = 0; n < targets_.size(); n++) {
		for (int m = 0; m < targets_[n].size(); m++) {
			targets_[n][m]->rot_sum_ *= rot_move_;
		}
	}

}











