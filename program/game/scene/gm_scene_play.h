#pragma once
#include "gm_scene_base.h"
#include "../model/gm_object.h"
#include "../model/gm_coordinate.h"
#include "../model/gm_object_mgr.h"

class GmCamera;
class ScenePlay : public SceneBase {
public :
	ScenePlay() {};
	~ScenePlay();

	GmCamera* camera_ = nullptr;

	ObjectMgr* co_mgr_;
	ObjectMgr* ro_mgr_;

	std::vector<Object* > obj_;
	Coordinate* object_;
	Coordinate* target_;
	Coordinate* target2_;
	
	float count_time_ = 0;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

