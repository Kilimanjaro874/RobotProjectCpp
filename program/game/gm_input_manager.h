#pragma once
#include "../../program/dxlib_ext/dxlib_ext.h"

class InputMgr {
public:
	// ---- プレイヤーの入力内容を取得＆操作量として格納していく ---- //
	InputMgr() {};
	~InputMgr() {};
	tnl::Vector3 _move;						// 水平移動量
	tnl::Vector3 _mouse_position_current;	// 現フレームのマウス位置
	tnl::Vector3 _mouse_position_past;		// 前フレームのマウス位置
	tnl::Vector3 _mouse_position_delta;		// 1フレーム間のマウス移動量

	void update(float delta_time);

};