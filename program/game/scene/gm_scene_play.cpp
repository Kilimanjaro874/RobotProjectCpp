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
	delete _hit_manager;
	for( auto tar : _target_obj_v) { delete tar; }
}


void ScenePlay::initialzie() {
	// ----- DxLib設定 ----- //
	GameManager* mgr = GameManager::GetInstance();
	_hit_manager = new HitManager();
	SetMouseDispFlag(false);			// マウス表示を消去
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPSでマウス位置を画面中央に固定
	// ----- ステージ生成 ----- //
	_floor_obj_li.resize(1);
	_floor_obj_li[0]= dxe::Mesh::CreatePlane({ 1500, 1500, 0 });
	_floor_obj_li[0]->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	_floor_obj_li[0]->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	_floor_obj_li[0]->pos_ += {0, -1, 0};
	// 背景
	_background = new Parts();
	_background->mesh_ = dxe::Mesh::CreateSphere(800);
	_background->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue2.bmp"));
																	
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
	// Target 
	_aim_pos = new Parts();
	_aim_pos->mesh_ = dxe::Mesh::CreateSphere(25);
	_aim_pos->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue.bmp"));

	// BGM
	mgr->_soundMgr->playSound(mgr->_soundMgr->bgm, 2, "", mgr->_soundMgr->loop);

	// TargetObjects
	setTargets();
	

	// sight
	_sight_UI_gh = LoadGraph("graphics/sight.png");
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	_controller->update(delta_time, _camera);
	// 右武器の当たり判定
	_hit_manager->CheckBulletTargetHit(_controller->_r_weapon->_bullets, _target_obj_v, mgr->_soundMgr);
	// 左武器の当たり判定
	_hit_manager->CheckBulletTargetHit(_controller->_l_weapon->_bullets, _target_obj_v, mgr->_soundMgr);


	// ターゲットとの当たり判定
	for(int i = 0; i < _target_obj_v.size(); i++) {
		_target_obj_v[i]->partsUpdate(delta_time);
		_target_obj_v[i]->move(delta_time);
		if (_target_obj_v[i]->_hp == 0) {
			delete _target_obj_v[i];
			_target_obj_v.erase(std::cbegin(_target_obj_v) + i);
		}
	}
	if (_target_obj_v.size() == 0) {
		_is_clear = true; 
		mgr->_clear_time = _clear_time;
		mgr->chengeScene(new SceneResult());
	}

	
	// sightDXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT
	if (!_is_clear) { _clear_time += delta_time; }	
}

void ScenePlay::render()
{
	_camera->update();
	
	//------------------------------------------------------------------
	//
	// Test
	//
	_robo->partsRenderTree(_robo, _camera);
	_background->mesh_->render(_camera);

	for (auto floor : _floor_obj_li) {
		floor->render(_camera);
	}

	//_target_obj_v[0]->partsRender(_camera);
	for (auto tar : _target_obj_v) {
		tar->partsRender(_camera);
	}
	DrawGridGround(_camera, 5, 300);
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 0.01, 0, _sight_UI_gh, true, false);
	DrawStringEx(50, 50, 1, "target Num = %d", _target_obj_v.size());
	DrawStringEx(50, 70, 1, "time = %5.2f", _clear_time);

}

void ScenePlay::setTargets() {
	// ----- フィールドに出現するターゲットの初期化 ----- //
	// * CSVデータから読み取れるよう後に変更する
	// 1
	std::vector<tnl::Vector3> points1;
	points1.push_back({ -50, 10, 500 });
	points1.push_back({ 50, 10, 500 });
	_target_obj_v.push_back(ShotTarget::init(points1, 2.5));
	// 2
	std::vector<tnl::Vector3> points2;
	points2.push_back({ 0, 150, 500 });
	points2.push_back({ 0, 50, 500 });
	_target_obj_v.push_back( ShotTarget::init(points2, 2.5));
	// 3
	std::vector<tnl::Vector3> points3;
	points3.push_back({ -100, 100, 500 });
	points3.push_back({ 100, 100, 500 });
	_target_obj_v.push_back(ShotTarget::init(points3, 2.5));
	// 4 
	std::vector<tnl::Vector3> points4;
	points4.push_back({ -500, 300, 0 });
	points4.push_back({ -500, 244, 144 });
	points4.push_back({ -500, 200, 200 });
	points4.push_back({ -500, 144, 144 });
	points4.push_back({ -500, 100, 0 });
	points4.push_back({ -500, 144, -144 });
	points4.push_back({ -500, 200, -200 });
	points4.push_back({ -500, 244, -144 });
	_target_obj_v.push_back(ShotTarget::init(points4, 3.0));
	// 5
	std::vector<tnl::Vector3> points5;
	points5.push_back({ 300, 300, 0 });
	points5.push_back({ 300, 50, 0 });
	points5.push_back({ 300, 50, 100 });
	points5.push_back({ 300, 50, -100 });
	points5.push_back({ 300, 200, 0 });
	points5.push_back({ 300, 200, 100 });
	points5.push_back({ 300, 200, -100 });
	points5.push_back({ 300, 200, 0 });
	_target_obj_v.push_back(ShotTarget::init(points5, 3.0));
	// 6
	std::vector<tnl::Vector3> points6;
	points6.push_back({ -300, 50, -300 });
	points6.push_back({ -300, 50, 300 });
	points6.push_back({ 300, 50, 300 });
	points6.push_back({ 300, 50, -300 });
	_target_obj_v.push_back(ShotTarget::init(points6, 5.0));
	
	// 7
	std::vector<tnl::Vector3> points7;
	points7.push_back({ 0, 60, -100 });
	points7.push_back({ 0, 10, -150 });
	points7.push_back({ 50, 60, -100 });
	points7.push_back({ 50, 10, -150 });
	_target_obj_v.push_back(ShotTarget::init(points7, 2.0));
	// 8
	std::vector<tnl::Vector3> points8;
	points8.push_back({ 50, 60, -100 });
	points8.push_back({ 50, 10, -150 });
	points8.push_back({ 100, 60, -100 });
	points8.push_back({ 100, 10, -150 });
	_target_obj_v.push_back(ShotTarget::init(points8, 2.0));
	// 9+
	std::vector<tnl::Vector3> points9;
	points9.push_back({ -50, 60, -100 });
	points9.push_back({ -50, 10, -150 });
	points9.push_back({ 0, 60, -100 });
	points9.push_back({ 0, 10, -150 });
	_target_obj_v.push_back(ShotTarget::init(points9, 2.0));

}