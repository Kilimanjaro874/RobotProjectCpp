#pragma once
#include<memory>
#include "gm_scene_base.h"
#include "../gm_actor.h"
#include "../gm_camera.h"
#include "../gm_assemble_repository.h"
#include "../gm_tps_camera_director.h"

//class GmCamera;
class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- member variables ----- //
	// ---- actors ---- //
	dxe::Camera* camera_ = nullptr;
	std::shared_ptr<tol::TPSCameraDirector> cam_director_ = nullptr;
	std::shared_ptr<tol::Actor> actor_ = nullptr;
	std::shared_ptr<tol::AssemRepo> assem_repo_ = nullptr;
	// --- etc. --- //
	std::shared_ptr<tol::Actor> back_ground_ = nullptr;
	std::shared_ptr<tol::Actor> floor_ = nullptr;

	// --- test --- //
	std::shared_ptr<tol::Actor> cam_target_ = nullptr;
	// ---- actor data : csv ---- //
	// --- robot --- //
	const std::string robot_actor_ = "gameCSV/Actor_RobotObj_01.csv";
	const std::string robot_ik_csv_ = "gameCSV/Actor_RobotObj_01_Ksetting.csv";
	const std::string targets_csv_ = "gameCSV/Actor_Targets.csv";
	// --- enemy --- //
	std::list < std::shared_ptr<tol::Actor>> targets_;
	// --- settings --- //
	bool is_window_active_ = true;		// player toched game window flag.
	// --- UI --- //
	int sight_gh_;						// Aiming sight
	int center_gh_;						// center sight
	// --- flag --- //
	bool game_clear = false;
	//// ----- member functions ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	// temp
	void targetsInit();
	void targetsHitCheck(float delta_time);
};
