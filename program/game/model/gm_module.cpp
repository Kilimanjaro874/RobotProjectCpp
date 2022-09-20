#include "gm_module.h"
//#include <cmath>

void Module::update(float delta_time) {
	// ----- LDK���{��A�{���f���ɕt���̃p�[�c�Q�̈ʒu�E�p�������X�V ----- //
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera * camera) {
	// ----- �{���f���ɕt���̃p�[�c�Q�����_�[(Model�̂܂�) ----- //
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::localDirectKinematics(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
	const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) {
	// ----- �Ǐ����^���w���{ ----- //	
	// �{���W���[����[i]�Ƃ��A[i-1]���W���[������̉�]��q_back, �ʒup_back����ɁA
	// ������W�no��x����Ƃ���(�ǂ̍��W�n����Ƃ��邩�͖{�N���X�^�p�ɂ��)�ʒu�E�p����
	// �X�V�����{��A���g�̉�]�̉e����[i+i]���W���[���։�]��rot_, �ʒupos_next�Ɋi�[����
	// �t�^���w�v�Z�������Ŏ��{
	// --- �ʒu�E��]���x�N�g���X�V ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// ���K��
	rotAi_.normalize();
	// --- LIK���{��A�{���W���[���̋Ǐ��I�ȉ�]i��i�����]�ʁA�����W���[���̈ʒu�v�Z ---
	tnl::Quaternion iqi = localInverseKinematics(delta_time, pos_e, pos_r);
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i���������Wx��֕ϊ�&�i�[�Fx��i
	rot_ = iqi * q_back;
	rot_sum_ *= rot_;		// �����p������̉�]�ʐώZ�l���i�[�F�p�[�c�̈ʒu�E�p���X�V�̂���
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i��link�ɍ�p��x��i��̍��W�ɕϊ��������W���[���̈ʒu��x��i��Ŋi�[����B
	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();
	pos_next = pos_ + link_dir * link_length;
}

void Module::localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
	const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) {
	// ----- �Ǐ����^���w���{ ----- //	
	// �{���W���[����[i]�Ƃ��A[i-1]���W���[������̉�]��q_back, �ʒup_back����ɁA
	// ������W�no��x����Ƃ���(�ǂ̍��W�n����Ƃ��邩�͖{�N���X�^�p�ɂ��)�ʒu�E�p����
	// �X�V�����{��A���g�̉�]�̉e����[i+i]���W���[���։�]��rot_, �ʒupos_next�Ɋi�[����
	// �t�^���w�v�Z�������Ŏ��{
	// --- �ʒu�E��]���x�N�g���X�V ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// ���K��
	rotAi_.normalize();
	// --- LIK���{��A�{���W���[���̋Ǐ��I�ȉ�]i��i�����]�ʁA�����W���[���̈ʒu�v�Z ---
	tnl::Quaternion iqi = localInverseKinematics(delta_time, pos_e, pos_r);
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i���������Wx��֕ϊ�&�i�[�Fx��i
	rot_ = iqi * q_back;
	rot_sum_ *= rot_;		// �����p������̉�]�ʐώZ�l���i�[�F�p�[�c�̈ʒu�E�p���X�V�̂���
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i��link�ɍ�p��x��i��̍��W�ɕϊ��������W���[���̈ʒu��x��i��Ŋi�[����B
	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();
	pos_next = pos_ + link_dir * link_length;
}

void Module::localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- �Ǐ����^���w���{ ----- //	
	// �{���W���[����[i]�Ƃ��A[i-1]���W���[������̉�]��q_back, �ʒup_back����ɁA
	// ������W�no��x����Ƃ���(�ǂ̍��W�n����Ƃ��邩�͖{�N���X�^�p�ɂ��)�ʒu�E�p����
	// �X�V�����{��A���g�̉�]�̉e����[i+i]���W���[���։�]��rot_, �ʒupos_next�Ɋi�[����
	// �t�^���w�v�Z�͎��{���Ȃ�
	// --- �ʒu�E��]���x�N�g���X�V ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// ���K��
	rotAi_.normalize();
	// --- LIK���� ---
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, 0);
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i���������Wx��֕ϊ�&�i�[�Fx��i
	rot_ = iqi * q_back;
	rot_sum_ *= rot_;		// �����p������̉�]�ʐώZ�l���i�[�F�p�[�c�̈ʒu�E�p���X�V�̂���
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i��link�ɍ�p��x��i��̍��W�ɕϊ��������W���[���̈ʒu��x��i��Ŋi�[����B
	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();
	pos_next = pos_ + link_dir * link_length;
}