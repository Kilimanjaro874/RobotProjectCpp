#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	//// ------ メンバ変数 ------ ////
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 _free_look_angle_xy = { tnl::ToRadian(30), 0, 0 };
	float _target_distance = 50;
	// ----- 焦点への単位ベクトル ----- //
	tnl::Vector3 _focus_dir = { 0, 0, 1 };		// 初期値
	tnl::Vector3 _focus_dir_tmp = _focus_dir;	// Update中の値
	tnl::Vector3 _aim_pos;
	float _aim_distance = 1000;

	//// ------ メンバ関数 ------ ////
	void update();
};

