#pragma once
#include "gm_parts.h"
#include "gm_face_vector.h"

class Module {
public:
	Module() {};
	virtual ~Module() {

	}
	// ----- Render ----- //
	std::vector<Parts*> parts_;
	// ----- Parameters ----- //
	int id_;		// �Q�Ɨp
	int hierarchy_;	// �K�w�Q�Ɨp

	// ----- Direct Kinematics : DK�ɕK�v�ȕϐ� ----- //
	struct dk_setting {
		tnl::Vector3 dir_r_n_;	// �����W���[���ւ̕����P�ʃx�N�g��
		float dir_r_length_;	// �����W���[���ւ̋����̑傫���i�[
	};
	std::vector<dk_setting> dk_s_v_;	// DK�Z�b�e�B���O�̃x�N�g���^
	tnl::Vector3 pos_o_;			// ���[���h���W�n�̖{���W���[��i�̌��_�ʒu
	tnl::Vector3 in_rot_axis_;		// �{���W���[��i�̉�]���P�ʃx�N�g���F�{���W���[�����_�ɗ��Ă�i�����l)
	tnl::Vector3 in_dir_z_;			// �{���W���[��i��z�������P�ʃx�N�g��(�����l)
	tnl::Vector3 in_dir_x_;			// �{���W���[��i��x�������P�ʃx�N�g��(�����l)
	tnl::Quaternion rot_tmp_;		// IK�v�Z���A1�t���[���Ԃ̂ݗL���ȃN�H�[�^�j�I��
	tnl::Quaternion rot_sum_;		// ������Ԃ���{���W���[��i�̉�]�ʂ�\���N�H�[�^�j�I��

	// ---- Inverse Kinematics : IK�ɕK�v�ȕϐ� ----- //
	struct ik_setting {
		int ik_type_;			// IK�ŉ����A���S���Y���̎�ނ��i�[
		float kp_;				// dth���ߏ�ȉ�]�ƂȂ�Ȃ��悤�}��������萔(0~1.0);
		FaceVector* target_;	// �ڕW�ʒu�N���X
		FaceVector* object_;	// ����ΏۃN���X
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

	



};