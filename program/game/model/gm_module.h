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
	tnl::Quaternion rot_;	// LDK�F�{���W�̋Ǐ��I�ȉ�]�N�H�[�^�j�I��
	tnl::Vector3 xli_;		// LDK: �{���W���[����]�ɂ��㎲�֗^���镽�s�ړ��ʂ��i�[
	tnl::Quaternion rot_sum_;	// �����p������̍��v�̉�]��\���N�H�[�^�j�I��(�p�[�c�̈ʒu�E�p���X�V�Ɏg�p)
	tnl::Vector3 rotAi_;	// ���W���[���̉�]���x�N�g��(�{���W���[����i���_�ɑ΂��ė��Ă�^�p�Ƃ���)
	
	// ---- �V�F�[�_�[�n ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- ���^���w�������֐� ----- 
	virtual void localDirectKinematics(const tnl::Quaternion& q_back, const tnl::Vector3& l_back);

};