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
	// ----- DxLib�ݒ� ----- //
	GameManager* mgr = GameManager::GetInstance();
	_hit_manager = new HitManager();
	SetMouseDispFlag(false);			// �}�E�X�\��������
	SetMousePoint(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2);		// TPS�Ń}�E�X�ʒu����ʒ����ɌŒ�
	// ----- �X�e�[�W���� ----- //
	_floor_obj_li.resize(1);
	_floor_obj_li[0]= dxe::Mesh::CreatePlane({ 1500, 1500, 0 });
	_floor_obj_li[0]->setTexture(dxe::Texture::CreateFromFile("graphics/gray.bmp"));
	_floor_obj_li[0]->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	_floor_obj_li[0]->pos_ += {0, -1, 0};
	// �w�i
	_background = new Parts();
	_background->mesh_ = dxe::Mesh::CreateSphere(700);
	_background->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/blue2.bmp"));
																	
	// ----- ����N���X�ݒ� ----- //
	_camera = new GmCamera();
	_camera->pos_ = { 0, 150, -300 };

	//------------------------------------------------------------------
	//
	// Test
	//
	// ���샍�{�b�g����
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
	std::vector<tnl::Vector3> points1;
	points1.push_back({ 0, 10, 0 });
	points1.push_back({ 10, 10, 0 });
	_target_obj_v.resize(1);
	_target_obj_v[0] = ShotTarget::init(points1, 2.0);
	

	// sight
	_sight_UI_gh = LoadGraph("graphics/sight.png");
}

void ScenePlay::update(float delta_time)
{
	// ----- DxLib�ݒ� ----- //
	

	GameManager* mgr = GameManager::GetInstance();


	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	//------------------------------------------------------------------
	//
	// Test
	//
	_controller->update(delta_time, _camera);
	// �E����̓����蔻��
	_hit_manager->CheckBulletTargetHit(_controller->_r_weapon->_bullets, _target_obj_v, mgr->_soundMgr);
	// ������̓����蔻��
	_hit_manager->CheckBulletTargetHit(_controller->_l_weapon->_bullets, _target_obj_v, mgr->_soundMgr);


	_target_obj_v[0]->partsUpdate(delta_time);
	_target_obj_v[0]->move(delta_time);
	
	
	// sightDXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT
	
	
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

	_target_obj_v[0]->partsRender(_camera);
	DrawGridGround(_camera, 5, 300);
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 0.01, 0, _sight_UI_gh, true, false);
}
