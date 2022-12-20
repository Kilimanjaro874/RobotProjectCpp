#pragma once
#include "gm_module.h"

class ShotTarget : public Module{
public:

	std::vector<tnl::Vector3> _move_points;
	float _circle_size = 5;
	float _move_time = 0;
	float _move_time_count = 0;
	int _move_point_num = 0;
	float _distance = 0;
	float _hp = 100;
	bool _is_hit = false;

	static ShotTarget* init(std::vector<tnl::Vector3> points, float move_time);
	void move(float delta_time);
};