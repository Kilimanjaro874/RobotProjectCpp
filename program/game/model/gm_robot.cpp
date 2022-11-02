#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- �ʒu pos, �p�� rot �փ��{�b�g�𐶐� ----- //
	

	// ---- 0. Robot[id = 0] : �p�����[�^������ ---- //
	Robot* rob = new Robot();
	rob->InitParams(0, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// rob->SelfDK(pos, rot);				// ���W�n�̍X�V: �Ō�ɂ܂Ƃ߂Ď��s����΂悢�̂ł�? 
	// DK �p�����[�^������
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 50, 0}, 1, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
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
	bb_z->dk_s_v_.resize(2);	// body, r_leg, l_leg, object�p
	bb_z->dk_s_v_[0] = { 200, {0, 25, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_body
	bb_z->dk_s_v_[1] = { 800, {0, 25, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_body
	// Parts
	Parts* bb_z_bone = new Parts();
	bb_z_bone->mesh_ = dxe::Mesh::CreateCylinder(5, 50);
	bb_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bb_z->parts_.push_back(bb_z_bone);


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
	bb_z->dk_s_v_[0] = { 500, {0, 25, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_head
	bo_z->dk_s_v_[1] = { 600, {25, -25, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for e_r_arm
	bo_z->dk_s_v_[2] = { 700, {-25, -25, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) }; // for e_l_arm
	bo_z->dk_s_v_[3] = { 820, {25, 0, 0}, 1, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };	// for object
	// Parts
	Parts* bo_z_bone = new Parts();
	bo_z_bone->mesh_ = dxe::Mesh::CreateCylinder(5, 50);
	bo_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bo_z->parts_.push_back(bo_z_bone);

	// ---- 3. right Arm ---- //
	rob->modules_[e_r_arm].resize(3);

	// --- 3.1. e_sho_x --- //
	Module* sho_x = new Module();
	// �q�ݒ�
	bo_z->next.resize(4);
	bo_z->next[0] = sho_x;
	// �e�ݒ�
	sho_x->back = bo_z;
	// �p�����[�^������
	sho_x->InitParams(600, { 1, 0, 0 }, tnl::Quaternion::RotationAxis({ 1, 0, 0 }, 0));
	sho_x->dk_s_v_.resize(1);
	sho_x->dk_s_v_[0] = { 601, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(sho_x->in_rot_axis_, 0) };

	// --- 3.2. e_sho_y --- //
	Module* sho_y = new Module();
	// �q�ݒ�
	sho_x->next.resize(1);
	sho_x->next[0] = sho_y;
	// �e�ݒ�
	sho_y->back = sho_x;
	// �p�����[�^������
	sho_y->InitParams(601, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	sho_y->dk_s_v_.resize(1);
	sho_y->dk_s_v_[0] = { 601, {0, 0, 1}, 30, tnl::Quaternion::RotationAxis(sho_y->in_rot_axis_, 0) };
	// Parts
	//Parts* 

	// --- 3.3. e_arm_x --- //
	Module* arm_x = new Module();
	// �q�ݒ�
	sho_y->next.resize(1);
	sho_y->next[0] = arm_x;
	// �e�ݒ�
	arm_x->back = sho_y;
	// �p�����[�^������
	arm_x->InitParams(602, { 0, 0, 1 }, tnl::Quaternion::RotationAxis({ 0, 0, 1 }, 0));
	arm_x->dk_s_v_.resize(1);
	arm_x->dk_s_v_[0] = { 603, {0, 0, 0}, 1, tnl::Quaternion::RotationAxis(arm_x->in_rot_axis_, 0) };





	// ----- ���{�S�Ẵp�[�c�̏����ʒu�E�p���𔽉f ----- //
	rob->rob_dk_s_v_.resize(1);
	tnl::Vector3 tmp_pos_dir = pos;
	tmp_pos_dir.normalize();
	float pos_length = pos.length();
	rob->rob_dk_s_v_[0] = {0, tmp_pos_dir, pos_length, rot};
	rob->AllInitDK(rob, rob->rob_dk_s_v_);
	rob->updateTree(rob, 0);

	return rob;
}

void Robot::mode01_init(float delta_time) {

}

void Robot::mode01_update(float delta_time) {

}

void Robot::move(float delta_time) {

}












