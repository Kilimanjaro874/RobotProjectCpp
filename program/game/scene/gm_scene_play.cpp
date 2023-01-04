#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete co_mgr_;
	for (auto m : mod_) delete m;
	delete object_;
	delete object2_;
	delete target_;
	delete target2_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	co_mgr_ = new CoordinateMgr();
	mod_.resize(7);

	for (int i = 0; i < 7; i++) {
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
	// set object target
	target_ = new Coordinate();
	target_->setCoordinate(
		1, "target[1]",
		{ 0, 0, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	target_->setViewCoorinate(1, 10);
	co_mgr_->registrateOrigine(target_, CoordinateMgr::co_type::target);

	target2_ = new Coordinate();
	target2_->setCoordinate(
		1, "target[2]",
		{ 40, 0, -40 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	target2_->setViewCoorinate(1, 10);
	co_mgr_->registrateCoordinate(1, "", target2_, CoordinateMgr::co_type::target);




	// object origine
	object_ = new Coordinate();
	object_->setCoordinate(
		1, "object[1]",
		{ 0, 0, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	object_->setViewCoorinate(1, 10);

	co_mgr_->registrateOrigine(object_, CoordinateMgr::co_type::object);
	auto tmp_attach_coord = co_mgr_->getRegistratedCoordinate(6, "", CoordinateMgr::co_type::normal);
	tmp_attach_coord->setChildAndDKInit(object_, Coordinate::attach_type::relative);
	// object mover




	// set IK setting
	std::vector<CoordinateMgr::coord_id_name_ik_st_> c_ik_st;
	c_ik_st.push_back({ 0, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 1, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 2, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 3, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 4, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 5, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	c_ik_st.push_back({ 6, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.2} });
	co_mgr_->registrateIKCoordinate(&c_ik_st);

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
	//co_mgr_->viewCoordinateState(CoordinateMgr::co_type::normal, CoordinateMgr::view_param::pos);
	//co_mgr_->viewCoordinateState(CoordinateMgr::co_type::target, CoordinateMgr::view_param::pos);
	co_mgr_->viewCoordinateState(CoordinateMgr::co_type::object, CoordinateMgr::view_param::pos);

	/*DrawStringEx(50, 50, -1, "scene play");
	DrawStringEx(50, 70, -1, "camera [ © : A ] [ ª : W ] [ ¨ : D ] [ « : S ]");
	DrawStringEx(50, 90, -1, "camera [ ‰“ : Z ] [ ‹ß : X ] ");
	DrawStringEx(50, 120, -1, "character [ ¶ : © ] [ ‰œ : ª ] [ ‰E : ¨ ] [ è‘O : « ] ");*/
}
