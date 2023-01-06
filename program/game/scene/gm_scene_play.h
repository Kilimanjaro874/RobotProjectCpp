#pragma once
#include "gm_scene_base.h"
#include "../model/gm_coordinate.h"
#include "../model/gm_module.h"
#include "../model/gm_coordinate_mgr.h"
#include "../model/gm_robot_mgr.h"

class GmCamera;
class ScenePlay : public SceneBase {
public :
	ScenePlay() {};
	~ScenePlay();

	GmCamera* camera_ = nullptr;

	CoordinateMgr* co_mgr_;
	RobotMgr* ro_mgr_;

	std::vector<Module* > mod_;
	Coordinate* object_;
	Coordinate* target_;
	Coordinate* target2_;
	
	float count_time_ = 0;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

