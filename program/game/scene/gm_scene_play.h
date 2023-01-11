#pragma once
#include<memory>
#include "gm_scene_base.h"
//#include "../gm_assemble.h"
//#include "../gm_assemble_repository.h"
#include "../gm_actor.h"
#include "../gm_camera.h"
#include "../gm_assemble_repository.h"

class Actor;
class GmCamera;
class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- member variables ----- //
	GmCamera* camera_ = nullptr;
	tol::Actor* actor_;
	tol::AssemRepo* assem_repo_ = nullptr;
	//tol::Assemble* test_ = nullptr;
	//// ----- member functions ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

