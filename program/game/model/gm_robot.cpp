#include "gm_robot.h"
#include <string>
#include <fstream>
#include <sstream>

Robot* Robot::Create(const tnl::Vector3 pos, const tnl::Quaternion rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	Robot* rob = new Robot();
	// ---- rob : プレイヤーの操作を受け付ける。そのための初期化実施 ---- //
	rob->init(rob, 1, "rob_ref_coord", pos, { 0, 1, 0 }, rot);
	rob->getModuleDataCSV(rob, "RP_ModuleSet001.csv");


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

void Robot::getModuleDataCSV(Robot* rob, std::string csv_path) {
	// ---- モジュールの構築情報をCSVから取得 ---- //
	std::string str_buf;
	std::string str_conma_buf;
	
	std::string str[102][19];
	int i = 0;
	int j = 0;
	
	// --- csvを開く ---
	std::ifstream ifs(csv_path);
	if (!ifs) {
		printf("error! File can't opened"); 
	}
	while (std::getline(ifs,  str_buf)) {
		std::string tmp = "";
		std::istringstream stream(str_buf);
		
		while (std::getline(stream, tmp, ','))
		{
			str[i][j] = tmp;
			j++;
		}
		j = 0;
		i++;
	}

	for (int i = 2; i < 102; i++) {
		if (str[i][0] == "") { continue; }
		int attachi_id = stoi(str[i][0]);
		int id = stoi(str[i][1]);
		std::string name = str[i][2];
		tnl::Vector3 pos = { stof(str[i][3]), stof(str[i][4]), stof(str[i][5]) };
		tnl::Vector3 rot_axis = { stof(str[i][6]), stof(str[i][7]), stof(str[i][8]) };
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis(rot_axis, tnl::ToRadian(stof(str[i][9])));
		tnl::Vector3 dirz = { stof(str[i][10]), stof(str[i][11]), stof(str[i][12]) };
		tnl::Vector3 dirx = { stof(str[i][13]), stof(str[i][14]), stof(str[i][15]) };
		Module* mod = Module::createModule(id, name, pos, rot_axis, rot, dirz, dirx);
		rob->attachModuleTree(attachi_id, "", mod);
	}
	
	printf("deb");

}
