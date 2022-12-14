#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../../dxlib_ext/dxlib_ext.h"


ScenePlay::~ScenePlay() {
	delete _camera;
	delete _robo;
	delete _controller;
	delete _background;
	delete _aim_pos;
}


void ScenePlay::initialzie() {
	// ----- DxLib設定 ----- //
	SetMouseDispFlag(false);			// マウス表示を消去
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPSでマウス位置を画面中央に固定
	// ----- 自作クラス設定 ----- //
	_camera = new GmCamera();
	_camera->pos_ = { 0, 150, -300 };

	//------------------------------------------------------------------
	//
	// Test
	//
	// 操作ロボット生成
	_robo = Robot::Create({ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));
	_robo->partsUpdateTree(_robo, 0);
	_controller = new RobotCont(_robo);

	_targets[0]._pos = { 5, 10, 50 };
	_targets[0]._rot_axis = { 0, 0, 1 };
	_targets[0]._angle = tnl::ToRadian(1);
	_targets[0]._rot = tnl::Quaternion::RotationAxis(_targets[0]._rot_axis, _targets[0]._angle);
	_targets[1]._pos = { -5, 10, 50 };
	_targets[1]._rot_axis = { 0, 0, -1 };
	_targets[1]._angle = tnl::ToRadian(1);
	_targets[1]._rot = tnl::Quaternion::RotationAxis(_targets[1]._rot_axis, _targets[1]._angle);

	// 背景
	_background = new Parts();
	_background->mesh_ = dxe::Mesh::CreateSphere(500);
	_background->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));
	// Target 
	_aim_pos = new Parts();
	_aim_pos->mesh_ = dxe::Mesh::CreateSphere(25);
	_aim_pos->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));

}

void ScenePlay::update(float delta_time)
{
	// ----- DxLib設定 ----- //
	

	GameManager* mgr = GameManager::GetInstance();


	//------------------------------------------------------------------
	//
	// カメラ制御
	//
	/*tnl::Vector3 rot[4] = {
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
	}*/
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	//------------------------------------------------------------------
	//
	// Test
	//
	_controller->update(delta_time, _camera);
	//_robo->directKinematicsTree(_robo, _robo->_dk_input);
	_targets[0]._pos = _targets[0]._pos.TransformCoord(_targets[0]._pos, _targets[0]._rot);
	_targets[1]._pos = _targets[1]._pos.TransformCoord(_targets[1]._pos, _targets[1]._rot);
	_robo->TranlateTree(930, "", _targets[0]._pos, _robo->absolute);
	_robo->setEffectIKTree(930, "", false);
	_robo->TranlateTree(940, "", _targets[1]._pos, _robo->absolute);
	_robo->setEffectIKTree(940, "", false);

	
	//_robo->directKinematicsAndIKTree(_robo, _robo->_dk_input, delta_time);
	//_robo->partsUpdateTree(_robo, delta_time);

	//_targets[0]._rot *= tnl::Quaternion::RotationAxis(_targets[0]._rot_axis, _targets[0]._angle);
	
	
	//_aim_pos->mesh_->pos_ = _camera->_aim_pos;
	
	
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
	//_background->mesh_->render(_camera);

	
	//tnl::Vector3 _aim_poss = _camera->_aim_pos;
	//DrawStringEx(50, 70, -1, "f = %f, %f, %f", _aim_poss.x, _aim_poss.y, _aim_poss.z);
	//_aim_pos->mesh_->render(_camera);

}
