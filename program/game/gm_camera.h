#pragma once
#include "../dxlib_ext/dxlib_ext_camera.h"

class GmCamera : public dxe::Camera {
public :
	GmCamera() : dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT) {}
	~GmCamera(){}
	tnl::Vector3 free_look_angle_xy_ = { 0, tnl::ToRadian(-90), 0};
	float target_distance_ = 200;

	void update();
};

