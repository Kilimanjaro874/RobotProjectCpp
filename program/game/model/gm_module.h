#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	// ----- Render ----- //
	std::vector<Parts*> parts_;
	bool is_render_ = true;		// �`�悷�邩�ۂ�
	// ----- Parameters ----- //
	int id_;		// �Q�Ɨp
	Module* back;				// �e���W���[��
	std::vector<Module*> next;	// �q���W���[��
	// ----- Direct Kinematics : DK�ɕK�v�ȕϐ� ----- //
	struct dk_setting {
		int id_;				// �{dk_setting���󂯎�郂�W���[��id_���w��
		tnl::Vector3 dir_r_n_;	// �����W���[���ւ̕����P�ʃx�N�g��
		float dir_r_length_;	// �����W���[���ւ̋����̑傫���i�[
		tnl::Quaternion q_r_n_;	// �����W���[���ւ̉�]��
	};
	std::vector<dk_setting> dk_s_v_;	// DK�Z�b�e�B���O�̃x�N�g���^
	std::vector<dk_setting> in_dk_s_v_;	// DK�Z�b�e�B���O�̊�����
	tnl::Vector3 pos_o_;			// ���[���h���W�n�̖{���W���[��i�̌��_�ʒu
	tnl::Vector3 in_rot_axis_;		// �{���W���[��i�̉�]���P�ʃx�N�g���F�{���W���[�����_�ɗ��Ă�i�����l)
	tnl::Vector3 rot_axis_;			// ��]���P�ʃx�N�g��
	tnl::Vector3 in_dir_z_;			// �{���W���[��i��z�������P�ʃx�N�g��(�����l)
	tnl::Vector3 dir_z_;			// z�������P�ʃx�N�g��
	tnl::Vector3 in_dir_x_;			// �{���W���[��i��x�������P�ʃx�N�g��(�����l)
	tnl::Vector3 dir_x_;			// x�������P�ʃx�N�g��
	tnl::Quaternion rot_tmp_;		// IK�v�Z���A1�t���[���Ԃ̂ݗL���ȃN�H�[�^�j�I��
	tnl::Quaternion rot_sum_;		// ������Ԃ���{���W���[��i�̉�]�ʂ�\���N�H�[�^�j�I��

	// ---- Inverse Kinematics : IK�ɕK�v�ȕϐ� ----- //
	struct ik_setting {
		int ik_type_;			// IK�ŉ����A���S���Y���̎�ނ��i�[
		float kp_;				// dth���ߏ�ȉ�]�ƂȂ�Ȃ��悤�}��������萔(0~1.0);
		Module* target_;	// �ڕW�ʒu�N���X
		Module* object_;	// ����ΏۃN���X
	};
	std::vector<ik_setting> ik_s_v_;

	enum ik_type {
		pos_to_pos,
		dirz_to_pos,
		dirx_to_pos,
		dirz_to_dirz,
		dirx_to_dirx,
		dirz_to_dirx,
		dirx_to_dirz
	};

	// ---- �����o�֐� ---- //
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	void InitParams(int id, tnl::Vector3 rot_axis, tnl::Quaternion rot_sum, 
		tnl::Vector3 dir_z = {0, 0, 1}, tnl::Vector3 dir_x = {1, 0, 0});
	void InitDK(const std::vector<dk_setting>& dks);
	void SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot);
	void DKwithIK(float delta_time, const std::vector<dk_setting>& dks);
	// test
	void AllInitDK(const Module* mod, const std::vector<dk_setting>& dks);
	void updateTree(const Module* mod, float delta_time);
	void renderTree(const Module* mod, dxe::Camera* camera);
	void AttachModule(Module* mod, Module* attach_mod, int id);
	void SetIKParams(Module* mod, int id, Module* target, Module* object,
		int ikType = pos_to_pos, float kp = 0.4);
	void AllupdateIK(const Module* mod, float delta_time);
};