#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};


	// ---- Direct Kinematics : DK�ɕK�v�ȕϐ� ---- //
	tnl::Vector3 pos_a_;		// �G�[�W�F���g���W�nx��ai����̖{���W���[�����W�n���_�ʒu
	tnl::Vector3 pos_o_;		// ���[���h���Wo������̖{���W���[��i�̍��W�n���_�ʒu
	tnl::Vector3 dir_z_;		// �{���W���[��i���烂�W���[��i+i�ւ̕����P�ʃx�N�g�����`(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 dir_x_;		// �{���W���[��i�p������̂��߂̎p�������P�ʃx�N�g�����`(�p��IK�v�Z�Ɏg�p)
	tnl::Vector3 rot_axis;		// ���W���[��i�̉�]���P�ʃx�N�g���F�{���W���[�����_�ɗ��Ă鎖�Ƃ���
	float link_length_;			// dir_z_�̑傫�����i�[�F���W���[��i���_���烂�W���[��i+i�܂ł̒������`
	tnl::Quaternion rot_tmp;	// IK�v�Z���Ao����ɋL�q���ꂽ�A1�t���[���ԂŖ{���W���[�������ꎞ�I�ȉ�]�ʂ��i�[
	tnl::Quaternion rot_sum;	// �{���W���[���������p������ǂꂾ����]���������i�[���Ă����B
	std::vector<float> kp_num;	// IK�F�����p�x�v�Z�̍ۂɎg�p������萔 ���ڕW�ʒu�E�p������������ꍇ�ɂ��Ή��̂���vector�\��
	
	

};