#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	//// ------ �����o�ϐ� ------ ////
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 free_look_angle_xy_ = { tnl::ToRadian(30), 0, 0 };
	float target_distance_ = 50;
	// ----- �œ_�ւ̒P�ʃx�N�g�� ----- //
	tnl::Vector3 focus_dir_ = { 0, 0, 1 };		// �����l
	tnl::Vector3 focus_dir_tmp_ = focus_dir_;	// Update���̒l
	//tnl::Vector3 _aim_pos;
	//float _aim_distance = 1000;

	//// ------ �����o�֐� ------ ////
	void update();
};

