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
	GameManager* mgr = GameManager::GetInstance();

	SetMouseDispFlag(false);			// マウス表示を消去
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPSでマウス位置を画面中央に固定
	// ----- ステージ生成 ----- //
	_floor_obj_li.resize(1);
	_floor_obj_li[0]= dxe::Mesh::CreatePlane({ 1000, 1000, 0 });
	_floor_obj_li[0]->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	_floor_obj_li[0]->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
																	
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
	_controller = new RobotCont(_robo, mgr->_soundMgr);
	// 背景
	_background = new Parts();
	_background->mesh_ = dxe::Mesh::CreateSphere(500);
	_background->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue2.bmp"));
	// Target 
	_aim_pos = new Parts();
	_aim_pos->mesh_ = dxe::Mesh::CreateSphere(25);
	_aim_pos->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));

	// BGM
	mgr->_soundMgr->playSound(mgr->_soundMgr->bgm, 2, "", mgr->_soundMgr->loop);

}

void ScenePlay::update(float delta_time)
{
	// ----- DxLib設定 ----- //
	

	GameManager* mgr = GameManager::GetInstance();


	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	//------------------------------------------------------------------
	//
	// Test
	//
	_controller->update(delta_time, _camera);
	
	
	
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
	_background->mesh_->render(_camera);

	for (auto floor : _floor_obj_li) {
		floor->render(_camera);
	}

}
