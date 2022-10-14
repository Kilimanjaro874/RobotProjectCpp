#pragma once
#include "gm_parts.h"
#include "gm_module.h"

class FaceVector : public Module{
	// ----- �L��x�N�g�����` ----- //
	// �L��x�N�g���F���{�b�g�H�w�ꕔ�E�G�Ŏg���p��B����_pos_��2���̒��s�����P�ʃx�N�g�����`���鎖�ŁA
	// �Q�[���I�u�W�F�N�g�̈ʒu�E�p����{�N���X�ŕ\�����鎖���ł���B(dir_y_���~�������dir_x_, dir_z_���O�ς���Ηǂ�)
	// Module IK�v�Z�̐���ڕW�ʒu�E�p�� or ����Ώۈʒu�E�p���̕\���̂��߁A�{�N���X���g�p����B
public:
	// ----- Render ----- //
	enum {
		e_origine,
		e_dir_z,
		e_dir_x,
		e_parts_max
	};
	float sphere_size_ = 2.0;
	float bar_length_ = 20.0;
	// ----- �����o�֐� ----- //
	static FaceVector* Create(int id, tnl::Vector3 pos, tnl::Quaternion rot);
};