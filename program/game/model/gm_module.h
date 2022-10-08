#pragma once
#include "gm_parts.h"
#include "gm_faceVec.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	int id_;						// �Q�Ɨp

	enum ik_type {
		pos_to_pos,
		dirz_to_pos,
		dirx_to_pos,
		dirz_to_dirz,
		dirx_to_dirx,
		dirz_to_dirx,
		dirx_to_dirz
	};
	// ----- Inverse Kinnematics : IK�ɕK�v�ȕϐ� ---- //
	struct ik_setting
	{
		int ik_type;		// IK�ŉ����^�C�v(1 ~ 5)
		float kp;			// dth���ߏ�Ȓl�ƂȂ�Ȃ��悤�}��������萔
		FaceVec* target;	// �ڕW�ʒu�E�p���̃|�C���^
		FaceVec* object;	// ���䂷��I�u�W�F�N�g�̈ʒu�E�p���̃|�C���^
	};
	std::vector<ik_setting> ik_settings_;

	// ---- Direct Kinematics : DK�ɕK�v�ȕϐ� ---- //
	tnl::Vector3 pos_o_;			// ���[���h���Wo������̖{���W���[��i�̍��W�n���_�ʒu
	tnl::Vector3 pos_o_next_;		// ���[���h���W�n�F�����W���[���̈ʒu���i�[
	tnl::Vector3 dir_z_;			// �{���W���[��i���烂�W���[��i+i�ւ̕����P�ʃx�N�g�����`(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 init_dir_z_;		// �{���W���[��i���烂�W���[��i+i�ւ̕����P�ʃx�N�g�����`�E�������(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 dir_x_;			// �{���W���[��i�p������̂��߂̎p�������P�ʃx�N�g�����`(�p��IK�v�Z�Ɏg�p)
	tnl::Vector3 init_dir_x_;		// �{���W���[��i�̎p����`�ׂ̈̒P�ʃx�N�g�����`�E�������(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 rot_axis_;			// ���W���[��i�̉�]���P�ʃx�N�g���F�{���W���[�����_�ɗ��Ă鎖�Ƃ���
	tnl::Vector3 init_rot_axis_;	// �{���W���[��i�̉�]���P�ʃx�N�g���E������Ԃ��`
	float link_length_;				// dir_z_�̑傫�����i�[�F���W���[��i���_���烂�W���[��i+i�܂ł̒������`
	tnl::Quaternion rot_tmp_;		// IK�v�Z���Ao����ɋL�q���ꂽ�A1�t���[���ԂŖ{���W���[�������ꎞ�I�ȉ�]�ʂ��i�[
	tnl::Quaternion rot_sum_;		// �{���W���[���������p������ǂꂾ����]���������i�[���Ă����B
	// ---- Render ----- //
	std::vector<Parts*> parts_;
	// ---- �����o�֐� ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
	void DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	tnl::Quaternion InverseKinematics(float delta_time);
	void DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back);

};