#pragma once
#include "gm_parts.h"

class FaceVector {
	// ----- �L��x�N�g�����` ----- //
	// �L��x�N�g���F���{�b�g�H�w�ꕔ�E�G�Ŏg���p��B����_pos_��2���̒��s�����P�ʃx�N�g�����`���鎖�ŁA
	// �Q�[���I�u�W�F�N�g�̈ʒu�E�p����{�N���X�ŕ\�����鎖���ł���B(dir_y_���~�������dir_x_, dir_z_���O�ς���Ηǂ�)
	// Module IK�v�Z�̐���ڕW�ʒu�E�p�� or ����Ώۈʒu�E�p���̕\���̂��߁A�{�N���X���g�p����B
public:
	FaceVector() {};
	virtual ~FaceVector() {

	}
	std::vector<Parts*> parts_;		// �p�[�c�N���X
	enum {
		e_origine_,
		
	};
};