#pragma once
#include "gm_module.h"

class ShotTarget : public Module{
public:

	std::vector<tnl::Vector3> _move_points;
	float _circle_size = 8;
	float _move_time = 0;
	float _move_time_count = 0;
	int _move_point_num = 0;
	float _distance = 0;
	float _hp = 100;
	bool _is_hit = false;
	float _hit_count = 0;
	float _hit_time = 0.2;

	static ShotTarget* init(std::vector<tnl::Vector3> points, float move_time);
	void partsRender(dxe::Camera* camera) override;
	void move(float delta_time);
	void hit(float damage);
};