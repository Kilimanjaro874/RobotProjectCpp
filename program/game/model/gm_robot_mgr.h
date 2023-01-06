#pragma once
#include "gm_coordinate_mgr.h"


class RobotMgr : public CoordinateMgr {
public:
	RobotMgr() { init(); };
	~RobotMgr() {};
	void init() override;
	//void update(float delta_time) override;
	
};