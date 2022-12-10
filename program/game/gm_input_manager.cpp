#include "gm_input_manager.h"

void InputMgr::update(float delta_time) {
	// ---- �v���C���[�ɗ^���鑀��ʂ��擾 ---- //
	// ���Z�b�g
	_move = { 0, 0, 0 };
	_mouse_position_current = { 0, 0, 0 };
	_mouse_position_delta = { 0, 0, 0 };
	// �O��
	if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) {_move.y = 1;}
	else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {_move.y = -1;}
	// ���E
	if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {_move.x = 1;}
	else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {_move.x = -1;}
	
	// �}�E�X
	_mouse_position_current = tnl::Input::GetMousePosition();
	_mouse_position_delta = _mouse_position_current - _mouse_position_past;
	_mouse_position_past = _mouse_position_current;
	DrawStringEx(50, 50, -1, "%2.5f, %2.5f", _mouse_position_delta.x, _mouse_position_delta.y);

}