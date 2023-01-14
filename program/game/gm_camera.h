#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	//// ------ メンバ変数 ------ ////
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 free_look_angle_xy_ = { tnl::ToRadian(30), 0, 0 };
	float target_distance_ = 50;
	// ----- 焦点への単位ベクトル ----- //
	tnl::Vector3 focus_dir_ = { 0, 0, 1 };		// 初期値
	tnl::Vector3 focus_dir_tmp_ = focus_dir_;	// Update中の値
	//tnl::Vector3 _aim_pos;
	//float _aim_distance = 1000;

	//// ------ メンバ関数 ------ ////
	void update();
};

