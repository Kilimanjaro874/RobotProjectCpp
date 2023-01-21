#pragma once
#include<memory>
#include "gm_scene_base.h"
#include "../gm_actor.h"
#include "../gm_camera.h"
#include "../gm_assemble_repository.h"

class GmCamera;
class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- member variables ----- //
	GmCamera* camera_ = nullptr;
	std::shared_ptr<tol::Actor> actor_;
	std::shared_ptr<tol::AssemRepo> assem_repo_;
	// ---- actor data : csv ---- //
	// --- robot --- //
	const std::string robot_actor_ = "gameCSV/Actor_RobotObj_01.csv";
	const std::string robot_ik_csv_ = "gameCSV/Actor_RobotObj_01_Ksetting.csv";

	//// ----- member functions ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};
