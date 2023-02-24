#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"

// test 
#include "../gm_physics_handler.h"
#include "../gm_pid_vel_controller.h"
#include "../gm_pid_rot_controller.h"
#include "../gm_restraint.h"

//static float  dth = 0;
ScenePlay::~ScenePlay() {
	delete camera_;
}

void ScenePlay::initialzie() {
	// ---- Dxlib settings [start] ---- //
	SetAlwaysRunFlag(false);		// Window inactive : continue game operation.
	// ---- Dxlib settings [end] ---- //
	GameManager* mgr = GameManager::GetInstance();
	
	// --- Create : Assemble Repository --- //
	assem_repo_ =  tol::AssemRepo::Create();

	// --- Actor Settings --- //
	actor_ = tol::Actor::Create(assem_repo_, robot_actor_, robot_ik_csv_);
	// -- attach compornents -- //
	std::shared_ptr<tol::PhysicsHandler> ph_handler =
		std::make_shared<tol::PhysicsHandler>(tol::PhysicsHandler(500.0, 500.0 * 2*2/8, 2.0, tnl::ToRadian(45)));
	std::shared_ptr<tol::PIDVelController> pid_cont =
		std::make_shared<tol::PIDVelController>(tol::PIDVelController(1.0, 1.0, 1800.0, 1855.0, 300.0));
	std::shared_ptr<tol::PIDRotController> pid_rot_cont =
		std::make_shared<tol::PIDRotController>(tol::PIDRotController(1.0, 150.0, 1.0, 40.0));
	actor_->setPhysicsHandler(ph_handler);
	actor_->setPIDVelController(pid_cont);
	actor_->setPIDRotController(pid_rot_cont);
	// -- attach weapon component to right&left arms -- //
	auto r_arm_tar = actor_->getObjectTree(2300, "");
	auto r_arm_tar_assem = r_arm_tar->getAssemble();
	r_arm_tar_assem->setCoordinateView(r_arm_tar, 20, 2);

	// --- Camera Settings --- //
	// -- Create : Camera Target -> chase the actor -- //
	cam_target_ = tol::Actor::Create(assem_repo_);
	auto cam_assem = cam_target_->getAssemble();
	//cam_assem->setCoordinateView(cam_target_, 1.0, 0.05);
	// -- attach compornents -- //
	std::shared_ptr<tol::PhysicsHandler> ph_cam_phy = std::make_shared<tol::PhysicsHandler>(tol::PhysicsHandler(5.0, 5.0 * 1*1/8, 100, tnl::ToRadian(90)));
	std::shared_ptr<tol::PIDPosController> pid_cam_cont = std::make_shared<tol::PIDPosController>(tol::PIDPosController(0.2, 0.005, 10.1));
	std::shared_ptr<tol::Restraint> re_cam = std::make_shared<tol::Restraint>(tol::Restraint());
	re_cam->init(actor_, 0, "camera_restraint", tol::Restraint::restraint_type::rot_as_rot);
	auto cam_target_kine = cam_target_->getKinematics();
	cam_target_->setPhysicsHandler(ph_cam_phy);
	cam_target_->setPIDPosController(pid_cam_cont);
	cam_target_kine->setRestraint(re_cam);
	// -- Create : Camera & Director -- //
	camera_ = new dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
	cam_director_ = std::make_shared<tol::TPSCameraDirector>(tol::TPSCameraDirector(camera_, { 0, 10, -20 }, cam_target_));
	
}

void ScenePlay::update(float delta_time)
{
	// -- process : when window active -- //
	if (is_window_active_) {
		GameManager* mgr = GameManager::GetInstance();
		// - move test start - //
		// translate
		tnl::Vector3 input = { 0, 0, 0 };
		if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			input += { 1, 0, 0 };
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			input += { -1, 0, 0 };
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			input += { 0, 0, 1 };
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			input += { 0, 0, -1 };
		}
		tnl::Vector3 input_pad = tnl::Input::GetLeftStick();
		if (input_pad.length() > 0.1) {
			actor_->pidVellContUpdate(delta_time, { input_pad.x, 0, -input_pad.y });		// give player control effect.
		}
		else {
			actor_->pidVellContUpdate(delta_time, input);
		}

		// test 

		// --- test --- //
		auto actor_cod = actor_->getCoordinate();
		tnl::Vector3 target_pos = actor_cod->getPos() + tnl::Vector3(0, 10, 0);
		cam_target_->pidPosContUpdate(delta_time, target_pos);
		cam_target_->updateTree(delta_time);

		auto cam_target_cod = cam_target_->getCoordinate();
		cam_director_->update(delta_time, camera_, cam_target_);


		actor_->pidRotContUpdate(delta_time, cam_director_->getForcusPos());

		auto r_arm_tar = actor_->getObjectTree(2300, "");
		r_arm_tar->Translate(cam_director_->getForcusPos(), true);

		auto l_arm_tar = actor_->getObjectTree(2400, "");
		l_arm_tar->Translate(cam_director_->getForcusPos(), true);
		
		// - move test end - //

		// general process
		actor_->updateTree(delta_time);

		
		

		

		
	}
	// -- process : when window nonactive -- //
	else {

	}
	
	// ---- Dxlib settings [start] ---- //
	if (tnl::Input::IsKeyDown(eKeys::KB_ESCAPE)) {
		is_window_active_ = false;
	}
	if (is_window_active_) {
		// --- for TPS mouse aiming : clear mouse cursor & fixed to center of screen. --- //
		SetMouseDispFlag(true);		// clear of mouse cursor.
		SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);
	}
	if (!is_window_active_) {
		// --- release mouse cursor --- //
		SetMouseDispFlag(true);		// appearance of mouse cursor.
		if (tnl::Input::IsMouseDown(tnl::Input::eMouse::LEFT)) {
			// back to game (TPS mouse aiming)
			int x, y;
			GetMousePoint(&x, &y);
			if (0 < x && x < DXE_WINDOW_WIDTH && 0 < y && y < DXE_WINDOW_HEIGHT) {
				is_window_active_ = true;
				SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// reset mousePosition immediately.
			}
		}
	}
	// ---- Dxlib settings [end] ---- //
}

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 5, 300);
	actor_->renderTree(camera_);
	// --- test --- //
	cam_target_->renderTree(camera_);
}
