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
	// ---- get instance ---- //
	//camera_director_ = std::make_shared<tol::TPSCamDirector>(tol::TPSCamDirector());
	//camera_ = new GmCamera();
	//camera_->pos_ = { 0, 150, -300 };
	


	assem_repo_ =  tol::AssemRepo::Create();
	actor_ = tol::Actor::Create(assem_repo_, robot_actor_, robot_ik_csv_);

	// debug start 
	auto ac_assem_ = actor_->getAssemble();
	ac_assem_->setCoordinateView(actor_,2.5, 0.1);
	// debug end 
	
	// --- Actor Control test --- //
	std::shared_ptr<tol::PhysicsHandler> ph_handler =
		std::make_shared<tol::PhysicsHandler>(tol::PhysicsHandler(500.0, 500.0 * 2*2/8, 100, tnl::ToRadian(90)));
	std::shared_ptr<tol::PIDVelController> pid_cont =
		std::make_shared<tol::PIDVelController>(tol::PIDVelController(2.0, 2.0, 1500.0, 1555.0, 200.0));
	std::shared_ptr<tol::PIDRotController> pid_rot_cont =
		std::make_shared<tol::PIDRotController>(tol::PIDRotController(1.0, 150.0, 20.0, 50.0));
	actor_->setPhysicsHandler(ph_handler);
	actor_->setPIDVelController(pid_cont);
	actor_->setPIDRotController(pid_rot_cont);
	// --- test --- //
	cam_target_ = tol::Actor::Create(assem_repo_);
	auto cam_assem = cam_target_->getAssemble();
	cam_assem->setCoordinateView(cam_target_, 5.0, 0.3);
	// attach classes
	std::shared_ptr<tol::PhysicsHandler> ph_cam_phy = std::make_shared<tol::PhysicsHandler>(tol::PhysicsHandler(5.0, 5.0 * 1*1/8, 100, tnl::ToRadian(90)));
	std::shared_ptr<tol::PIDPosController> pid_cam_cont = std::make_shared<tol::PIDPosController>(tol::PIDPosController(0.0002, 0.005, 0.001));
	cam_target_->setPhysicsHandler(ph_cam_phy);
	cam_target_->setPIDPosController(pid_cam_cont);
	std::shared_ptr<tol::Restraint> re_cam = std::make_shared<tol::Restraint>(tol::Restraint());
	re_cam->init(actor_, 0, "camera_restraint", tol::Restraint::restraint_type::rot_as_rot);
	auto cam_target_kine = cam_target_->getKinematics();
	cam_target_kine->setRestraint(re_cam);

	// --- camera test --- //
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
		if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			input += { 1, 0, 0 };
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			input += { -1, 0, 0 };
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
			input += { 0, 0, 1 };
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
			input += { 0, 0, -1 };
		}
		tnl::Vector3 input_pad = tnl::Input::GetLeftStick();
		if (input_pad.length() > 0.1) {
			actor_->pidVellContUpdate(delta_time, { input_pad.x, 0, -input_pad.y });		// give player control effect.
		}
		else {
			actor_->pidVellContUpdate(delta_time, { 0, 0, 0 });
		}
		//actor_->Rotation(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1)), true);
		// rotation
		
		// - move test end - //

		// general process
		actor_->updateTree(delta_time);

		// --- test --- //
		auto actor_cod = actor_->getCoordinate();
		tnl::Vector3 target_pos = actor_cod->getPos() + tnl::Vector3(0, 10, 0);
		cam_target_->pidPosContUpdate(delta_time, target_pos);
		cam_target_->updateTree(delta_time);

		//------------------------------------------------------------------
		//
		// ƒJƒƒ‰§Œä
		//
		/*tnl::Vector3 rot[4] = {
			{ 0, tnl::ToRadian(1.0f), 0 },
			{ 0, -tnl::ToRadian(1.0f), 0 },
			{ tnl::ToRadian(1.0f), 0, 0 },
			{ -tnl::ToRadian(1.0f), 0, 0 } };
		tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
			camera_->free_look_angle_xy_ += rot[idx];
			}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);
		if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
			camera_->target_distance_ += 1.0f;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_X)) {
			camera_->target_distance_ -= 1.0f;
		}*/

		// test 
		auto cam_target_cod = cam_target_->getCoordinate();
		tnl::Vector3 cam_pos = -cam_target_cod->getDirZ() * 20.0f;
		//camera_->target_ = cam_target_cod->getPos() + tnl::Vector3(0, 11, 0);
		//camera_->pos_ = cam_pos + tnl::Vector3(0, 20, 0);
		//camera_director_->update(delta_time, camera_, cam_target_, cam_pos);
		cam_director_->update(delta_time, camera_, cam_target_);
		actor_->pidRotContUpdate(delta_time, cam_director_->getForcusPos());
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
	//camera_director_->renderUpdate();
	
	DrawGridGround(camera_, 5, 300);
	actor_->renderTree(camera_);

	// --- test --- //
	cam_target_->renderTree(camera_);
}
