#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	//// ------ メンバ変数 ------ ////
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 free_look_angle_xy_ = { tnl::ToRadian(30), 0, 0 };
	float target_distance_ = 50;
	// -----

	//// ------ メンバ関数 ------ ////
	void update();
};

