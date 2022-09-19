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
	tnl::Vector3 pos_;		// ���Wx��i����̈ʒu
	tnl::Vector3 pos_next;	// �����W���[���̈ʒu���v�Z����
	float link_length;		// �{���W���[���̃����N����
	tnl::Vector3 link_dir;			// �����N�̕���
	tnl::Quaternion rot_;	// LDK�F�{���W�̋Ǐ��I�ȉ�]�N�H�[�^�j�I��
	tnl::Quaternion rot_sum_;	// �����p������̍��v�̉�]��\���N�H�[�^�j�I��(�p�[�c�̈ʒu�E�p���X�V�Ɏg�p)
	tnl::Vector3 rotAi_;	// ���W���[���̉�]���x�N�g��(�{���W���[����i���_�ɑ΂��ė��Ă�^�p�Ƃ���)
	// ---- �t�^���w�v�Z�ɕK�v�ȕϐ� ----
	float kp_;				// �Ǐ��t�^���w�̔����p�x�v�Z�ɗp������萔kp
	
	// ---- �V�F�[�_�[�n ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- ���^���w�������֐�:LDK(������LIK���������s) -----
	virtual void localDirectKinematics(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back, 
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// --- �t�^���w�������֐�:LIK ----
	virtual tnl::Quaternion localInverseKinematics(float delta_time, const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	
	// ---- �Ǐ����^���w�v�Z�F�t�^���w�v�Z�������Ŏ��{ ---- //
	virtual void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// ---- �Ǐ����^���w�v�Z�F�t�^���w�v�Z����
	virtual void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
};

