#pragma once
#include "../../program/dxlib_ext/dxlib_ext.h"

class InputMgr {
public:
	// ---- �v���C���[�̓��͓��e���擾������ʂƂ��Ċi�[���Ă��� ---- //
	InputMgr() {};
	~InputMgr() {};
	tnl::Vector3 _move;						// �����ړ���
	tnl::Vector3 _mouse_position_current;	// ���t���[���̃}�E�X�ʒu
	tnl::Vector3 _mouse_position_past;		// �O�t���[���̃}�E�X�ʒu
	tnl::Vector3 _mouse_position_delta;		// 1�t���[���Ԃ̃}�E�X�ړ���

	void update(float delta_time);

};