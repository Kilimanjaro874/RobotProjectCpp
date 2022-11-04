#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3 pos, const tnl::Quaternion rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	Robot* rob = new Robot();
	// ---- rob : プレイヤーの操作を受け付ける。そのための初期化実施 ---- //
	rob->init(rob, 1, "rob_ref_coord", pos, { 0, 1, 0 }, rot);
	// test : 四角形をアタッチする 
	Module* box = Module::createModule(2, "box1", { 0, 5, 0 }, { 0, 1, 0 }, 
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0)));
	box->setAxisView(0.3, 5);
	Parts* box_s = new Parts();
	box_s->mesh_ = dxe::Mesh::CreateBox(3);
	box_s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	box->_parts.push_back(box_s);

	rob->attachModule(rob, box);

	// test : 更に四角形をアタッチする
	Module* box2 = Module::createModule(3, "box2", { 0, 15, 0 }, { 0, 1, 0 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-45)));
	box2->setAxisView(0.3, 5);
	Parts* box_2s = new Parts();
	box_2s->mesh_ = dxe::Mesh::CreateBox(3);
	box_2s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	box2->_parts.push_back(box_2s);

	box->attachModule(box, box2);

	return rob;

}

void Robot::init(Robot* rob, int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis, tnl::Quaternion rot) {
	// ---- rob : プレイヤーの操作を受け付ける。そのための初期化実施 ---- //
	rob->_id = id;
	rob->_name = name;
	rob->_pos = pos;
	rob->_rot_axis = rot_axis;
	rob->_rot = rot;
	// --- 座標系の初期化 --- //
	rob->_dir_z = { 0, 0, 1 };
	rob->_dir_x = { 1, 0, 0 };
	// --- DKパラメータの初期化 --- //
	rob->_dk_input.resize(1);
	rob->_dk_input[0]._id = rob->_id;
	rob->_dk_input[0]._name = rob->_name;
	rob->_dk_st = rob->_dk_input;
}

