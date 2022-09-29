#pragma once
#include "gm_parts.h"

class FaceVec {
	// ----- �L��x�N�g�����` ----- //
	// �L��x�N�g���F���{�b�g�H�w�ꕔ�E�G�Ŏg���p��B����_pos_��2���̒��s�����P�ʃx�N�g�����`���鎖�ŁA
	// �Q�[���I�u�W�F�N�g�̈ʒu�E�p����{�N���X�ŕ\�����鎖���ł���B(dir_y_���~�������dir_x_, dir_z_���O�ς���Ηǂ�)
	// Module IK�v�Z�̐���ڕW�ʒu�E�p�� or ����Ώۈʒu�E�p���̕\���̂��߁A�{�N���X���g�p����B
	// ������������΁AParts�N���X��{�N���X�ɓo�^����Ηǂ��B
public:
	FaceVec() {};
	virtual ~FaceVec() {
		for (auto pts : parts_) delete pts;
	}
	// ---- �����o�ϐ� ---- //
	// --- for DK, IK --- 
	tnl::Vector3 pos_;		// �ʒu�i�ǂ̍��W�n���ɑ����邩�̓��[�U�[�̐ݒ莟��F�ʒu�Q�Ɨp)
	tnl::Vector3 dir_z_;	// �����P�ʃx�N�g��z(�l�Ԃł����A�l�����w��ڕW�֌��������̂��̕�����P�ʃx�N�g���Ƃ��ĕ\��:�p���Q�Ɨp)
	tnl::Vector3 dir_x_;	// �����P�ʃx�N�g��x(�l�����w�ɑ΂��A���s�����e�w�̕�����P�ʃx�N�g���Ƃ��ĕ\���F�p���Q�Ɨp)
	tnl::Quaternion rot_sum_;	// �{�N���X�����p������ǂꂾ����]���������i�[���Ă����B
	// --- for render --- 
	std::vector<Parts*> parts_;		// �p�[�c�N���X
	enum {
		e_origine_,
		e_dir_z_,
		e_dir_x_,
		e_parts_max
	};
	// ---- �����o�֐� ---- //
	// �K�v�ȋ@�\��s�x�A�ǉ����Ă����\��
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	static FaceVec* Create(tnl::Vector3 pos_);
	void Rotate(tnl::Quaternion rot_temp);
};