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
#include "../gm_weapon.h"

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
	ph_handler->setIsAffectedByGravity(true);	// affected by gravity.
	std::shared_ptr<tol::PIDVelController> pid_cont =
		std::make_shared<tol::PIDVelController>(tol::PIDVelController(1.0, 1.0, 1800.0, 1855.0, 300.0));
	std::shared_ptr<tol::PIDRotController> pid_rot_cont =
		std::make_shared<tol::PIDRotController>(tol::PIDRotController(1.0, 150.0, 1.0, 40.0));
	actor_->setPhysicsHandler(ph_handler);
	actor_->setPIDVelController(pid_cont);
	actor_->setPIDRotController(pid_rot_cont);
	// -- attach weapon component to right&left arms -- //
	// - right weapon - //
	auto r_arm_weapon = actor_->getObjectTree(1300, "RAEE01");
	auto r_bullet = std::make_shared<tol::Object>(tol::Object(-1, "bullet"));
	r_bullet->init();
	auto r_bullet_assem = assem_repo_->CopyAssemble(900, "machine_gun", false);
	r_bullet->setAssemble(r_bullet_assem);
	auto r_weapon = std::make_shared<tol::Weapon>(tol::Weapon(
		0.20, 20.0, 800.0, 1000.0, 1.0,
		tol::Weapon::fire_dir::up,
		tol::Weapon::bullet_dir::up,
		r_bullet
	));
	r_arm_weapon->setWeapon(r_weapon);
	// - left weapon - //
	auto l_arm_weapon = actor_->getObjectTree(1400, "LAEE01");
	auto l_bullet = std::make_shared<tol::Object>(tol::Object(-2, "bullet"));
	l_bullet->init();
	auto l_bullet_assem = assem_repo_->CopyAssemble(901, "rifle", false);
	l_bullet->setAssemble(l_bullet_assem);
	auto l_weapon = std::make_shared<tol::Weapon>(tol::Weapon(
		0.5, 40.0, 1200.0, 1500.0, 1.0,
		tol::Weapon::fire_dir::up,
		tol::Weapon::bullet_dir::up,
		l_bullet
	));
	l_arm_weapon->setWeapon(l_weapon);
	// -- aim target test -- //
	auto r_arm_tar = actor_->getObjectTree(2300, "");
	auto r_arm_tar_assem = r_arm_tar->getAssemble();
	//r_arm_tar_assem->setCoordinateView(r_arm_tar, 20, 2);

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
	cam_director_ = std::make_shared<tol::TPSCameraDirector>(tol::TPSCameraDirector(camera_, { 0, 15, -20 }, cam_target_));
	// tmp : test
	targetsInit();
	
	// --- UI --- //
	sight_gh_ = LoadGraph("graphics/sight3.png");
	// --- BGM --- //
	mgr->sound_mgr_->playSound(mgr->sound_mgr_->bgm, 2, "", mgr->sound_mgr_->loop);
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
		if (tnl::Input::IsKeyDown(eKeys::KB_SPACE)) {
			input += {0, 1, 0};
		}
		tnl::Vector3 input_pad = tnl::Input::GetLeftStick();
		if (input_pad.length() > 0.1) {
			actor_->pidVellContUpdate(delta_time, { input_pad.x, 0, -input_pad.y });		// give player control effect.
		}
		else {
			actor_->pidVellContUpdate(delta_time, input);
		}
		// --- fire test --- //
		auto r_arm_weapon = actor_->getObjectTree(1300, "RAEE01");
		if (tnl::Input::IsMouseDown(tnl::Input::eMouse::RIGHT)) {
			r_arm_weapon->getWeapon()->setFire(true);
		}
		auto l_arm_weapon = actor_->getObjectTree(1400, "LAEE01");
		if (tnl::Input::IsMouseDown(tnl::Input::eMouse::LEFT)) {
			l_arm_weapon->getWeapon()->setFire(true);
		}
		// -- set fired SE -- //
		if (r_arm_weapon->getWeapon()->getFired()) {
			mgr->sound_mgr_->playSound(mgr->sound_mgr_->se, 1, "", mgr->sound_mgr_->one_shot);
		}
		if (l_arm_weapon->getWeapon()->getFired()) {
			mgr->sound_mgr_->playSound(mgr->sound_mgr_->se, 2, "", mgr->sound_mgr_->one_shot);
		}

		// test 
		// --- aiming test --- //
		auto actor_cod = actor_->getCoordinate();
		tnl::Vector3 target_pos = actor_cod->getPos() + tnl::Vector3(0, 15, 0);
		cam_target_->pidPosContUpdate(delta_time, target_pos);
		cam_target_->updateTree(delta_time);
		// --- camera move --- //
		auto cam_target_cod = cam_target_->getCoordinate();
		cam_director_->update(delta_time, camera_, cam_target_);


		actor_->pidRotContUpdate(delta_time, cam_director_->getForcusPos());

		auto r_arm_tar = actor_->getObjectTree(2300, "");
		r_arm_tar->Translate(cam_director_->getForcusPos(), true);

		auto l_arm_tar = actor_->getObjectTree(2400, "");
		l_arm_tar->Translate(cam_director_->getForcusPos(), true);
		
		
		// --- target test --- //
		targetsHitCheck(delta_time);

		// - move test end - //

		// general process
		actor_->updateTree(delta_time);
		for (auto itr = targets_.begin(); itr != targets_.end();) {
			auto target = *itr;
			target->pidPosContUpdate(delta_time, { 0, 0, 0 });
			target->update(delta_time);
			itr++;
		}
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
		SetMouseDispFlag(false);		// clear of mouse cursor.
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
	for (auto itr = targets_.begin(); itr != targets_.end();) {
		auto target = *itr;
		target->render(camera_);
		itr++;
	}

	auto r_arm_tar = actor_->getObjectTree(2300, "");
	r_arm_tar->render(camera_);
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 0.05, 0, sight_gh_, true, false);
}


void ScenePlay::targetsInit() {
	// --- Create : target test --- //
	auto target = tol::Actor::Create(assem_repo_);
	target->getKinematics()->Translate(target, tnl::Vector3{ 100, 30, 0 }, true);
	auto target_assem = assem_repo_->CopyAssemble(1000, "BallEnemy");
	target->setAssemble(target_assem);
	std::shared_ptr<tol::CircleCollider> collider = std::make_shared<tol::CircleCollider>(tol::CircleCollider(500));
	target->setCircleCollider(collider);
	std::shared_ptr<tol::PhysicsHandler> target_phy = std::make_shared<tol::PhysicsHandler>(tol::PhysicsHandler(5.0, 5.0 * 1 * 1 / 8, 1, tnl::ToRadian(90)));
	std::shared_ptr<tol::PIDPosController> target_pos_cont = std::make_shared<tol::PIDPosController>(tol::PIDPosController(0.2, 0.5, 0.1));
	// pos automatic locations
	std::vector<tnl::Vector3> auto_locations;
	auto_locations.push_back(tnl::Vector3{ 100, 30, 0 });
	auto_locations.push_back(tnl::Vector3{ 100, 30, 100 });
	auto_locations.push_back(tnl::Vector3{ 200, 30, 200 });
	target_pos_cont->setAutomaticLocationUpdate(auto_locations, 2.0);
	target->setPhysicsHandler(target_phy);
	target->setPIDPosController(target_pos_cont);
	targets_.push_back(target);


}

void ScenePlay::targetsHitCheck(float delta_time) {
	
	auto r_arm_weapon = actor_->getObjectTree(1300, "RAEE01");
	std::list<std::shared_ptr<tol::Object>> r_bullets = r_arm_weapon->getWeapon()->getBullets();
	auto l_arm_weapon = actor_->getObjectTree(1400, "LAEE01");
	std::list<std::shared_ptr<tol::Object>> l_bullets = l_arm_weapon->getWeapon()->getBullets();

	for (auto itr = targets_.begin(); itr != targets_.end();) {
		(*itr)->update(delta_time);

		for (auto itr2 = r_bullets.begin(); itr2 != r_bullets.end();) {
			(*itr)->getCircleCollider()->hitCheck((*itr), (*itr2));
			itr2++;
		}
		for (auto itr3 = r_bullets.begin(); itr3 != r_bullets.end();) {
			(*itr)->getCircleCollider()->hitCheck((*itr), (*itr3));
			itr3++;
		}

		itr++;

	}
}