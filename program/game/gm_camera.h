#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	//// ------ �����o�ϐ� ------ ////
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 _free_look_angle_xy = { tnl::ToRadian(30), 0, 0 };
	float _target_distance = 50;
	// ----- �œ_�ւ̒P�ʃx�N�g�� ----- //
	tnl::Vector3 _focus_dir = { 0, 0, 1 };		// �����l
	tnl::Vector3 _focus_dir_tmp = _focus_dir;	// Update���̒l
	tnl::Vector3 _aim_pos;
	float _aim_distance = 1000;

	//// ------ �����o�֐� ------ ////
	void update();
};

