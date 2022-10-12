#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete sprite_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };

	sprite_ = new AnimSprite3D(camera_);
	sprite_->regist(32, 48, "walk_front", "graphics/c1_anim_up.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, 48, 0);
	sprite_->regist(32, 48, "walk_back", "graphics/c1_anim_down.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, 48, 0);
	sprite_->regist(32, 48, "walk_left", "graphics/c1_anim_left.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, 48, 0);
	sprite_->regist(32, 48, "walk_right", "graphics/c1_anim_right.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, 48, 0);

	sprite_->setCurrentAnim("walk_front");

}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	//------------------------------------------------------------------
	//
	// ˆÚ“®§Œä
	//
	int t = tnl::GetXzRegionPointAndOBB(
		camera_->pos_
		, sprite_->pos_
		, {32, 48, 32}
		, sprite_->rot_);

	std::string anim_names[4] = {
		"walk_back", "walk_right", "walk_front", "walk_left"
	};
	sprite_->setCurrentAnim(anim_names[t]);

	tnl::Vector3 move_v = { 0,0,0 };
	tnl::Vector3 dir[4] = {
		camera_->front().xz(), 
		camera_->right().xz(), 
		camera_->back().xz(),
		camera_->left().xz(), 
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		move_v += dir[idx];
	}, eKeys::KB_UP, eKeys::KB_RIGHT, eKeys::KB_DOWN, eKeys::KB_LEFT);

	if (tnl::Input::IsKeyDown(eKeys::KB_UP, eKeys::KB_RIGHT, eKeys::KB_DOWN, eKeys::KB_LEFT)) {
		move_v.normalize();
		sprite_->rot_.slerp( tnl::Quaternion::LookAtAxisY(sprite_->pos_, sprite_->pos_ + move_v), 0.3f);
		sprite_->pos_ += move_v * 2.0f;
	}

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
	camera_->target_ = sprite_->pos_;
	sprite_->update(delta_time);
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}
}

void ScenePlay::render()
{
	camera_->update();

	DrawGridGround(camera_, 50, 20);

	sprite_->render(camera_);
	DrawOBB(camera_, sprite_->pos_, sprite_->rot_, { 32, 48, 32 });


	DrawStringEx(50, 50, -1, "scene play");
	DrawStringEx(50, 70, -1, "camera [ © : A ] [ ª : W ] [ ¨ : D ] [ « : S ]");
	DrawStringEx(50, 90, -1, "camera [ ‰“ : Z ] [ ‹ß : X ] ");
	DrawStringEx(50, 120, -1, "character [ ¶ : © ] [ ‰œ : ª ] [ ‰E : ¨ ] [ è‘O : « ] ");

}
