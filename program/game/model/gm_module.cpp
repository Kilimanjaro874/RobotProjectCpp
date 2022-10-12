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

void Module::localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
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
	tnl::Quaternion iqi = localInverseKinematics(delta_time);
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

tnl::Quaternion Module::localInverseKinematics(float delta_time) {
	// ----- �Ǐ��t�^���w���{ ----- // 
	// �{���W���[���̉�]���ɐ����ȕ��ʏ�ŁA���ʒupos_e, �ڕW�ʒupos_r���߂Â����]��(�N�H�[�^�j�I���j�𐶐��A
	// �������A��]�ʂ͎����W���[���֎��X�Ƒ傫���`�d���Ă������߁A��]�ʂ������]�Ƃ��邽�ߔ��萔kp���悸�鎖�ŁA
	// ���ʒu���ڕW�ʒu�֑Q�߂��Ă����悤�ȋ����̎�����ڎw���B
	// �܂��A��]������̔����p�xdth�̓t���[�����[�g�ω��ŋ��������Ȃ��Ȃ��悤�A���ݎ��Ԃ��悶�Ă���(�W��60fps�z��)�B
	// �O�ό��ʁF�E��n
	// �� �v�Z�O��pos_r_, pos_e_���X�V���Ă�����
	// ---- �����p�x�v�Z -----
	float dth = 0;
	float dth_sum = 0;
	for (int i = 0; i < kp_num_.size(); i++) {
		tnl::Vector3 x = tnl::Vector3::Cross((pos_e_[i] - pos_), rotAi_);
		tnl::Vector3 y = tnl::Vector3::Cross((pos_r_[i] - pos_), rotAi_);
		// ��]��������Ɏ��ڕW�ʒu�A������ʒu������ꍇ�͔����p�x��0�Ƃ���
		dth = delta_time * 60.0 * kp_num_[i] * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length()
			, (float)-1, float(1)));
		if (!isfinite(dth)) { dth = 0; }
		// ---- ��]�����̌��� ---- 
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
		float sign = rotAi_.dot(axis) > 0 ? 1 : -1;
		dth *= sign;		// ��������
		dth_sum += dth;		// n��̋t�^���w�̌��ʂ�Ԃ�
	}

	// ---- �Ǐ��I(i��i)�ȉ�]�ʁF�N�H�[�^�j�I����Ԃ� ---- //
	return tnl::Quaternion::RotationAxis(rotAi_, dth_sum);

}