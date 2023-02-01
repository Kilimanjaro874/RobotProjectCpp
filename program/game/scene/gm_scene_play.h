#pragma once
#include<memory>
#include "gm_scene_base.h"
#include "../gm_actor.h"
#include "../gm_camera.h"
#include "../gm_assemble_repository.h"
#include "../gm_camera_director.h"



//class GmCamera;
class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- member variables ----- //
	GmCamera* camera_ = nullptr;
	std::shared_ptr<tol::TPSCamDirector> camera_director_ = nullptr;
	std::shared_ptr<tol::Actor> actor_ = nullptr;
	std::shared_ptr<tol::AssemRepo> assem_repo_ = nullptr;
	// --- test --- //
	std::shared_ptr<tol::Actor> cam_target_ = nullptr;
	// ---- actor data : csv ---- //
	// --- robot --- //

	const std::string robot_actor_ = "gameCSV/Actor_RobotObj_01.csv";
	const std::string robot_ik_csv_ = "gameCSV/Actor_RobotObj_01_Ksetting.csv";
	// --- settings --- //
	bool is_window_active_ = true;		// player toched game window flag.

	//// ----- member functions ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;
};
