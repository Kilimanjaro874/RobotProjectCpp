#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"


ScenePlay::~ScenePlay() {
	delete camera_;
	delete robo_;
	
	for (auto tar : targets_) { delete tar; }
	for (auto obj : objects_) {	delete obj;	}
	/*for (auto obj : obj_parts_) { delete obj; }
	delete parts_;*/
}


void ScenePlay::initialzie() {

	camera_ = new GmCamera();
	
	robo_ = Robot::Create({ 0, 0, 0 }, tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0));

	targets_.resize(2);
	targets_[0] = FaceVec::Create({ 0, 0, 100 });
	targets_[1] = FaceVec::Create({ 50, -30, 0 });
	for (auto tar : targets_) { tar->InitDK(robo_->pos_o_); }	// robo座標系にFix

	robo_->mode01_init(targets_);
	



	// test
	/*parts_ = new Parts();
	obj_parts_ = dxe::Mesh::CreateFromFileObj("graphics/OBJ/Sword.obj");*/
	//parts_->mesh_->CreateFromFileObj("graphics/OBJ/RedBoss.obj");
	//parts_->mesh_->setDefaultLightEnable(true);

}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();
	//------------------------------------------------------------------
	//
	// カメラ制御
	//
	tnl::Vector3 rot[4] = {
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
	}
	// --- シーン切り替え --- //
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	/*for (auto tar : targets_) {
		tar->update(delta_time);
	}*/


	// 試験動作
	robo_->update(delta_time);

	tnl::Vector3 tar1 = robo_->targets_[0]->pos_;
	tnl::Vector3 tar2 = robo_->targets_[1]->pos_;
	DrawStringEx(50, 10, -1, "TG1:x= %f, y= %f, z= %f", tar1.x, tar1.y, tar1.z);
	DrawStringEx(50, 25, -1, "TG2:x= %f, y= %f, z= %f", tar2.x, tar2.y, tar2.z);
	tnl::Vector3 obj1 = robo_->agents_[0]->cnt_objects_[0]->pos_;
	tnl::Vector3 obj2 = robo_->agents_[0]->cnt_objects_[1]->pos_;
	DrawStringEx(50, 40, -1, "EE1:x= %f, y= %f, z= %f", obj1.x, obj1.y, obj1.z);
	DrawStringEx(50, 55, -1, "EE2:x= %f, y= %f, z= %f", obj2.x, obj2.y, obj2.z);

	tnl::Vector3 pos_origin = { 0, 15, 0 };
	tnl::Vector3* pos;
	pos = &pos_origin;
	pos_origin += { 0, 1, 0 };
	DrawStringEx(50, 70, -1, "pos = %f, %f, %f", pos->x, pos->y, pos->z);
	


	//// target 操作 
	//if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
	//	targets_[0]->pos_ += tnl::Vector3{ 0, 10, 0 };
	//}
	//else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
	//	targets_[0]->pos_ += tnl::Vector3{ 0, -10, 0 };
	//}
	//else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT)) {
	//	targets_[0]->pos_ += tnl::Vector3{ 0, 0, 10 };
	//}
	//else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT)) {
	//	targets_[0]->pos_ += tnl::Vector3{ 0, 0, -10 };
	//}
	
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(camera_, 50, 20);
	

	robo_->render(camera_);

	for (int i = 0; i < 2; i++) {
		robo_->targets_[i]->render(camera_);
		robo_->agents_[0]->cnt_objects_[i]->render(camera_);
		
	}
	
	////parts_->mesh_->render(camera_);
	//for (auto obj : obj_parts_) {
	//	obj->render(camera_);
	//}
}
