#pragma once
#include "gm_parts.h"

class Module {
public:
	// ----- ���^���w�v�Z�@�\�������������f���̂悤�ȃC���[�W ----- //
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}

	// ---- ���^���w�v�Z�ɕK�v�ȕϐ� ----
	tnl::Vector3 pos_;			// �G�[�W�F���g���Wx��ai����̈ʒu
	tnl::Vector3 pos_next;		// �����W���[��[i+1]�̈ʒu���v�Z���i�[
	float link_length;			// �{���W���[���̃����N����
	tnl::Vector3 link_dir;		// �{���W���[�����玟�̃��W���[���ւ̕����P�ʃx�N�g��
	tnl::Quaternion rot_;		// LDK�F�{���W�̋Ǐ��I�ȉ�]�N�H�[�^�j�I��
	tnl::Quaternion rot_sum_;	// �����p������̍��v�̉�]��\���N�H�[�^�j�I��(�p�[�c�̈ʒu�E�p���X�V�Ɏg�p)�F�܂��A������֐ߓ���p�x�����̂��߂̏��ɂ��Ȃ�͂�
	tnl::Vector3 rotAi_;		// ���W���[���̉�]���x�N�g��(�{���W���[��x��i���_�ɑ΂��āB�{�P�ʃx�N�g������ɉ�]��^����)
	float kp_;					// �Ǐ��t�^���w�̔����p�x�v�Z�ɗp������萔kp
	std::vector<float> kp_num_;			// ���W���[�������p�x�v�Z���萔(�����̏ꍇ����) 
	std::vector<tnl::Vector3> pos_r_;	// ���W���[�����ڕW�ʒu(�����̏ꍇ����)
	std::vector<tnl::Vector3> pos_e_;	// pos_r�ɒǏ]�����������Q�ƈʒu(�����̏ꍇ����)
	
	// ---- �p�[�c�Ǘ� ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- ���^���w�������֐�:LDK(������LIK���������s) -----

	// --- �t�^���w�������֐�:LIK ----
	virtual tnl::Quaternion localInverseKinematics(float delta_time);
	
	// ---- �Ǐ����^���w�v�Z�F�t�^���w�v�Z�������Ŏ��{ ---- //
	virtual void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	// ---- �Ǐ����^���w�v�Z�F�t�^���w�v�Z����
	virtual void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	// ---- pr, pe�̃x�N�^�[�X�V ---- //
	//virtual void setTarget_EE(std::vector<tnl::Vector3> pos_r, std::vector<tnl::Vector3> pos_e);

};

