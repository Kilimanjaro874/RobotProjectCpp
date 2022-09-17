#include "gm_module.h"

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

void Module::localDirectKinematics(const tnl::Vector3& p_back, const tnl::Quaternion& q_back, const tnl::Vector3& l_back) {
	// ----- �Ǐ��I���^���w���{ ----- //	
	// �{���W���[����[i]�Ƃ��A[i-1]���W���[������̉�]��q_back, ���s�ړ���l_back����ɁA
	// ������W�n����Ƃ���(�ǂ̍��W�n����Ƃ��邩�͖{�N���X�^�p�ɂ��)�ʒu�E�p����
	// �X�V�����{��A���g�̉�]�̉e����[i+i]���W���[���։�]��q_i, ���s�ړ���l_i�Ƃ��ēn��
	// --- �ʒu�E��]���x�N�g���X�V ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	
	// ���K��
	rotAi_.normalize();
	

	// --- �t�^���w���� ---
	// test
	float dth = tnl::ToRadian(2.0);
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i�����]�ʁA���s�ړ��ʐ��� ---
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, dth);




	//tnl::Vector3 ili = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i���������Wx��֕ϊ�&�i�[�Fx��i
	rot_ = iqi * q_back;

	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();


	rot_sum_ *= rot_;		// �����p������̉�]�ʐώZ�l���i�[�F�p�[�c�̈ʒu�E�p���X�V�̂���
	// --- ���s�ړ��ʂ����l�A�Ǐ��I�ȉ�]�ɂ�镽�s�ړ��̉e����x��i��ŕ\�����i�[
	//xli_ = tnl::Vector3::TransformCoord(l_back, iqi) + ili;
	
	pos_next = pos_ + link_dir * link_length;

}
//
//void Module::localDirectKinematics(const tnl::Quaternion& q_back, const tnl::Vector3& l_back) {
//	// ----- �Ǐ��I���^���w���{ ----- //	
//	// �{���W���[����[i]�Ƃ��A[i-1]���W���[������̉�]��q_back, ���s�ړ���l_back����ɁA
//	// ������W�n����Ƃ���(�ǂ̍��W�n����Ƃ��邩�͖{�N���X�^�p�ɂ��)�ʒu�E�p����
//	// �X�V�����{��A���g�̉�]�̉e����[i+i]���W���[���։�]��q_i, ���s�ړ���l_i�Ƃ��ēn��
//	// --- �ʒu�E��]���x�N�g���X�V ---
//	pos_ = tnl::Vector3::TransformCoord(pos_, q_back) + l_back;
//	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
//	// --- �t�^���w���� ---
//	// test
//	float dth = tnl::ToRadian(10.0);
//	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i�����]�ʁA���s�ړ��ʐ��� ---
//	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, dth);
//	tnl::Vector3 ili = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
//	// --- �{���W���[���̋Ǐ��I�ȉ�]i��i���������Wx��֕ϊ�&�i�[�Fx��i
//	rot_ = iqi * q_back;
//	rot_sum_ *= rot_;		// �����p������̉�]�ʐώZ�l���i�[�F�p�[�c�̈ʒu�E�p���X�V�̂���
//	// --- ���s�ړ��ʂ����l�A�Ǐ��I�ȉ�]�ɂ�镽�s�ړ��̉e����x��i��ŕ\�����i�[
//	xli_ = tnl::Vector3::TransformCoord(l_back, iqi) + ili;
//
//}
//
