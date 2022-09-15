#pragma once
#include "gm_model.h"

class MdlRobotModule : public Model {
	// ----- 1�����N���̃��{�b�g���W���[���쐬(���f�����p��) ------ //
public:
	// --- �ЂƂ܂��P�p�[�c�݂̂ō\�� --- //
	enum {
		e_link01,		// �����N�p�[�c
		e_axis01,		// ��]���p�[�c
		e_axis02,		// ��]���p�[�c
		e_axis03,		// ��]���p�[�c
		e_parts_max
	};
	// ---- �����o�ϐ���` ---- //
	// --- DK�p�����[�^�ǉ� ---
	// pos_ : ���W���[�����_�̈ʒu�x�N�g���Ƃ��Ďg�p(o��i�)
	// rot_ : ���W���[���̋Ǐ��I�ȉ�]�Ƃ��Ďg�p(i��i�)
	tnl::Vector3 rotAi_;		// �������W���[����]��(o��i�)
	tnl::Vector3 posLi_;		// �������W���[�����玟�̃��W���[�����_�ւ̃x�N�g��(o��i�)

	void update(float delta_time) override;
	static MdlRobotModule* Create();
};