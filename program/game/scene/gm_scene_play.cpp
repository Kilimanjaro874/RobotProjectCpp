#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete co_mgr_;
	for (auto m : mod_) delete m;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	co_mgr_ = new CoordinateMgr();
	mod_.resize(3);
	coord_.resize(3);
	for (int i = 0; i < 3; i++) {
		float delta = 50.0 * (float)i;
		mod_[i] = new Module();
		mod_[i]->setCoordinate(
			i,
			"coord[" + std::to_string(i) + "]",
			{ delta, 0, 0 },
			{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
			{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
			tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0))
		);
		mod_[i]->setViewCoorinate(1, 10);
		Parts* p = new Parts();
		p->mesh_ = dxe::Mesh::CreateSphere(1);
		p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
		mod_[i]->setParts(p);

		if (i == 0) {
			co_mgr_->registrateOrigine(mod_[i], CoordinateMgr::co_type::normal); 
		}
		else {
			co_mgr_->registrateCoordinate(i - 1, "", mod_[i], CoordinateMgr::co_type::normal);
		}

	}



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
		camera_->free_look_angle_xy_ += rot[idx];
		}, eKeys::KB_A, eKeys::KB_D, eKeys::KB_W, eKeys::KB_S);

	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		camera_->target_distance_ += 1.0f;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_X)) {
		camera_->target_distance_ -= 1.0f;
	}
	camera_->target_ = mod_[0]->getPos();
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	co_mgr_->update(delta_time);

	//for (int i = 0; i < 10; i++) {
	//	float tmp = 0;
	//	tmp = tnl::ToRadian(1);
	//	coord_[i]->directKinematics(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tmp));
	//	coord_[i]->update(delta_time);
	//	//mod_[i]->directKinematics(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tmp));
	//	//mod_[i]->update(delta_time);
	//}

}
	

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 50, 20);

	/*for (int i = 0; i < 10; i++) {
		coord_[i]->render(camera_);
	}*/

	co_mgr_->render(camera_);
	co_mgr_->viewCoordinateState(CoordinateMgr::co_type::normal, CoordinateMgr::view_param::pos);

	/*DrawStringEx(50, 50, -1, "scene play");
	DrawStringEx(50, 70, -1, "camera [ © : A ] [ ª : W ] [ ¨ : D ] [ « : S ]");
	DrawStringEx(50, 90, -1, "camera [ ‰“ : Z ] [ ‹ß : X ] ");
	DrawStringEx(50, 120, -1, "character [ ¶ : © ] [ ‰œ : ª ] [ ‰E : ¨ ] [ è‘O : « ] ");*/
}
