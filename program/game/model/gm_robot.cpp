#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ----- �ʒu pos, �p�� rot �փ��{�b�g�𐶐� ----- //
	// ---- Robot[id = 0] : �p�����[�^������ ---- //
	Robot* rob = new Robot();
	rob->InitParams(0, { 0, 1, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	// rob->SelfDK(pos, rot);				// ���W�n�̍X�V: �Ō�ɂ܂Ƃ߂Ď��s����΂悢�̂ł�? 
	// DK �p�����[�^������
	rob->dk_s_v_.resize(1);
	rob->dk_s_v_[0] = { 100, {0, 50, 0}, 1, tnl::Quaternion::RotationAxis(rob->in_rot_axis_, 0) };
	// ���W���[���T�C�Y�ݒ�
	rob->modules_.resize(e_modules_max);

	// ---- 1. lower back : �p�����[�^������
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
	bb_y->dk_s_v_[0] = { 101, {0, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_y->in_rot_axis_, 0) };

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
	bb_x->dk_s_v_[0] = { 102, {0, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_x->in_rot_axis_, 0) };

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
	bb_z->dk_s_v_[0] = { 200, {0, 25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[1] = { 300, {25, -25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[2] = { 400, {-25, -25, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	bb_z->dk_s_v_[3] = { 800, {25, 0, 0}, 0, tnl::Quaternion::RotationAxis(bb_z->in_rot_axis_, 0) };
	// Parts
	Parts* bb_z_bone = new Parts();
	bb_z_bone->mesh_ = dxe::Mesh::CreateCylinder(5, 50);
	bb_z_bone->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	bb_z->parts_.push_back(bb_z_bone);
	

	// �p�[�c�̏����ʒu�E�p���𔽉f
	rob->rob_dk_s_v_.resize(1);
	tnl::Vector3 tmp_pos_dir = pos;
	tmp_pos_dir.normalize();
	float pos_length = pos.length();
	rob->rob_dk_s_v_[0] = {0, tmp_pos_dir, pos_length, rot};
	rob->AllInitDK(rob, rob->rob_dk_s_v_);

	return rob;
}

//void Robot::update(float delta_time) {
//	// ----- �e���W���[����update���s ----- //
//	for (int n = 0; n < modules_.size(); n++) {
//		for (auto mod : modules_[n]) {
//			mod->update(delta_time);
//		}
//	}
//}
//
//void Robot::render(dxe::Camera* camera) {
//	// ----- �e���W���[����render���s ----- //
//	for (int n = 0; n < modules_.size(); n++) {
//		for (auto mod : modules_[n]) {
//			mod->render(camera);
//		}
//	}
//}

void Robot::mode01_init(float delta_time) {

}

void Robot::mode01_update(float delta_time) {

}

void Robot::move(float delta_time) {

}












