#include "../gm_manager.h"
#include "../gm_camera.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete co_mgr_;
	delete ro_mgr_;
	for (auto o : obj_) { 
		delete o; 
	}
	delete object_;
	delete target_;
	delete target2_;
}


void ScenePlay::initialzie() {
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	co_mgr_ = new ObjectMgr();
	co_mgr_->init();
	ro_mgr_ = new ObjectMgr();
	ro_mgr_->init();
	obj_.resize(7);

	// attach Modules to Coordinate Mgr
	for (int i = 0; i < 7; i++) {
		float delta = 5.0 * (float)i;
		obj_[i] = new Object();
		obj_[i]->setCoordinate(
			i,
			"coord[" + std::to_string(i) + "]",
			{ delta, 0, 0 },
			{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
			{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
			tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0))
		);
		obj_[i]->setViewCoorinate(0.05, 0.5);
		Parts* p = new Parts();
		p->mesh_ = dxe::Mesh::CreateSphere(0.01);
		p->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
		obj_[i]->setParts(p);

		if (i == 0) {
			co_mgr_->registrateOrigine(obj_[i], ObjectMgr::co_type::normal);
		}
		else {
			co_mgr_->registrateCoordinate(i - 1, "", obj_[i], ObjectMgr::co_type::normal);
		}

	}

	// --- set object target (for IK) --- //
	// target
	target_ = new Coordinate();
	target_->setCoordinate(
		1, "target[1]",
		{ 0, 0, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	target_->setViewCoorinate(0.1, 0.5);
	co_mgr_->registrateOrigine(target_, ObjectMgr::co_type::target);

	target2_ = new Coordinate();
	target2_->setCoordinate(
		2, "target[2]",
		{ 100, 0, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	target2_->setViewCoorinate(0.1, 0.55);
	co_mgr_->registrateCoordinate(1, "", target2_, ObjectMgr::co_type::target);

	// object
	object_ = new Coordinate();
	object_->setCoordinate(
		1, "object[1]",
		{ 0, 0, 0 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	object_->setViewCoorinate(0.1, 0.5);
	co_mgr_->registrateOrigine(object_, ObjectMgr::co_type::object);

	// attach object to Robot Arm End Effector 
	auto tmp_attach_coord = co_mgr_->getRegistratedCoordinate(6, "", ObjectMgr::co_type::normal);
	tmp_attach_coord->setChildAndDKInit(object_, Coordinate::attach_type::relative);




	// --- init IK setting --- //
	std::vector<ObjectMgr::coord_id_name_ik_st_> c_ik_st;
	c_ik_st.push_back({ 0, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.20} });
	//c_ik_st.push_back({ 0, "", {object_, target2_, Coordinate::ik_type::dirx_as_dirx, 0.05} });
	//c_ik_st.push_back({ 0, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.05} });
	//c_ik_st.push_back({ 0, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.05} });
	
	c_ik_st.push_back({ 1, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.20} });
	//c_ik_st.push_back({ 1, "", {object_, target2_, Coordinate::ik_type::dirx_as_dirx, 0.05} });
	//c_ik_st.push_back({ 1, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.05} });
	//c_ik_st.push_back({ 1, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.05} });
	c_ik_st.push_back({ 2, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.08} });
	//c_ik_st.push_back({ 2, "", {object_, target2_, Coordinate::ik_type::dirx_as_dirx, 0.05} });
	//c_ik_st.push_back({ 2, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.05} });
	//c_ik_st.push_back({ 2, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.05} });
	c_ik_st.push_back({ 3, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.08} });
	//c_ik_st.push_back({ 3, "", {object_, target2_, Coordinate::ik_type::dirx_as_dirx, 0.01} });
	//c_ik_st.push_back({ 3, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.001} });
	//c_ik_st.push_back({ 3, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.001} });
	c_ik_st.push_back({ 4, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.025} });
	//c_ik_st.push_back({ 4, "", {object_, target2_, Coordinate::ik_type::dirz_look_pos, 0.001} });
	//c_ik_st.push_back({ 4, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.001} });
	//c_ik_st.push_back({ 4, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.001} });
	c_ik_st.push_back({ 5, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.005} });
	//c_ik_st.push_back({ 5, "", {object_, target2_, Coordinate::ik_type::dirz_look_pos, 0.001} });
	//c_ik_st.push_back({ 5, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.001} });
	//c_ik_st.push_back({ 5, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.001} });
	//c_ik_st.push_back({ 6, "", {object_, target2_, Coordinate::ik_type::pos_to_pos, 0.01} });
	//c_ik_st.push_back({ 6, "", {object_, target2_, Coordinate::ik_type::dirz_look_pos, 0.001} });
	c_ik_st.push_back({ 6, "", {object_, target2_, Coordinate::ik_type::diry_as_diry, 0.001} });
	c_ik_st.push_back({ 6, "", {object_, target2_, Coordinate::ik_type::dirz_as_dirz, 0.001} });
	co_mgr_->registrateIKCoordinate(&c_ik_st);	

}

void ScenePlay::update(float delta_time)
{
	count_time_ += delta_time;
	GameManager* mgr = GameManager::GetInstance();

	// --- Camera Control --- //
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
	camera_->target_ = obj_[0]->getPos();
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}

	
	// --- Move Target --- //
	tnl::Vector3 move = { 10 * sin(count_time_), 10, 10 * cos(count_time_) };
	// get target coordinate
	auto tmp_attach_target = co_mgr_->getRegistratedCoordinate(2, "", ObjectMgr::co_type::target);
	tmp_attach_target->setTranslate(move, Coordinate::attach_type::absolute);
	// set construction 
	auto tmp_attach_coord = co_mgr_->getRegistratedCoordinate(1, "", ObjectMgr::co_type::object);
	tmp_attach_coord->constraintAdd(tmp_attach_target, Coordinate::const_condition::rot_as_rot);
	// update all regstrated coordinate
	co_mgr_->update(delta_time);
	ro_mgr_->update(delta_time);
}
	

void ScenePlay::render()
{
	camera_->update();
	DrawGridGround(camera_, 5, 200);

	// renda all regstrated coordinate
	co_mgr_->render(camera_);
	ro_mgr_->render(camera_);
	ro_mgr_->viewCoordinateState(ObjectMgr::co_type::normal, ObjectMgr::view_param::pos);
	//co_mgr_->viewCoordinateState(ObjectMgr::co_type::normal, ObjectMgr::view_param::pos);
	
}
