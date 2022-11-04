#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"


ScenePlay::~ScenePlay() {
	delete _camera;
	delete _robo;
	delete _controller;
}


void ScenePlay::initialzie() {
	_camera = new GmCamera();
	_camera->pos_ = { 0, 150, -300 };

	//------------------------------------------------------------------
	//
	// Test
	//
	_robo = Robot::Create({ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	_robo->partsUpdateTree(_robo, 0);
	_controller = new RobotCont(_robo);
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	//------------------------------------------------------------------
	//
	// ƒJƒƒ‰§Œä
	//
	tnl::Vector3 rot[4] = {
		{ 0, tnl::ToRadian(1.0f), 0 },
		{ 0, -tnl::ToRadian(1.0f), 0 },
		{ tnl::ToRadian(1.0f), 0, 0 },
		{ -tnl::ToRadian(1.0f), 0, 0 } };
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		_camera->free_look_angle_xy_ += rot[idx];
	}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);

	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		_camera->target_distance_ += 1.0f;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_X)) {
		_camera->target_distance_ -= 1.0f;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	//------------------------------------------------------------------
	//
	// Test
	//
	_controller->update(delta_time);
	_robo->directKinematicsTree(_robo, _robo->_dk_input);
	_robo->partsUpdateTree(_robo, delta_time);

	if (tnl::Input::IsKeyDown(eKeys::KB_SPACE)) {
		_robo->removeModuleTree(_robo, 2, "box1", true);
	}

}

void ScenePlay::render()
{
	_camera->update();
	DrawGridGround(_camera, 1, 200);

	//------------------------------------------------------------------
	//
	// Test
	//
	_robo->partsRenderTree(_robo, _camera);

}
