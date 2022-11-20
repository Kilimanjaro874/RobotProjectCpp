#include "gm_robot.h"
#include <string>
#include <fstream>
#include <sstream>

Robot* Robot::Create(const tnl::Vector3 pos, const tnl::Quaternion rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	Robot* rob = new Robot();
	// ---- rob : プレイヤーの操作を受け付ける。そのための初期化実施 ---- //
	rob->init(rob, 1, "rob_ref_coord", pos, { 0, 1, 0 }, rot);
	rob->setAxisView(0.2, 2.0);
	rob->getModuleDataCSV(rob, "RP_ModuleTestRArm001.csv");
	rob->getIKSetDataCSV(rob, "RP_ModuleIKTestRArm001.csv");
	Module* mod = new Module;
	mod = rob->getModulePointerTree(mod, 4, "");
	// ---- parts取付機能実装前：こちらでパーツ生成＆アッタッチ ---- //
	
	
	
	// ---- RArmXX: 右腕のメッシュ生成 ---- //
	int const sRA = 3;
	Parts* RA0X_J[sRA];
	Parts* RA0X_A[sRA];
	int sRAid[sRA] = { 301, 303, 306 };
	float sRAsize[sRA] = { 2, 2, 1 };
	for (int i = 0; i < sRA; i++) {
		RA0X_J[i] = new Parts();
		RA0X_J[i]->mesh_ = dxe::Mesh::CreateSphere(0.3);
		RA0X_J[i]->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
		rob->attachPartsTree(sRAid[i], "", RA0X_J[i]);
		RA0X_A[i] = new Parts();
		RA0X_A[i]->mesh_ = dxe::Mesh::CreateCylinder(0.2, sRAsize[i]);
		RA0X_A[i]->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
		RA0X_A[i]->ofs_pos_ += {0, - sRAsize[i]/2, 0};
		rob->attachPartsTree(sRAid[i], "", RA0X_A[i]);
	}

	// -----LArmXX: 左腕のメッシュ生成 ---- //
	
	
	// Mod id 2


	//Parts* j1 = new Parts();
	//j1->mesh_ = dxe::Mesh::CreateSphere(1);
	//j1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//rob->attachPartsTree(2, "", j1);
	//Parts* a1 = new Parts();
	//a1->mesh_ = dxe::Mesh::CreateCylinder(0.5, 5);
	//a1->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//a1->ofs_pos_ += {0, 2.5, 0};
	//rob->attachPartsTree(2, "", a1);
	//// Mod id 3
	//Parts* j2 = new Parts();
	//j2->mesh_ = dxe::Mesh::CreateSphere(1);
	//j2->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//rob->attachPartsTree(3, "", j2);
	//Parts* a2 = new Parts();
	//a2->mesh_ = dxe::Mesh::CreateCylinder(0.5, 5);
	//a2->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//a2->ofs_pos_ += {0, 2.5, 0};
	//rob->attachPartsTree(3, "", a2);
	//// Mod id 4
	//Parts* j3 = new Parts();
	//j3->mesh_ = dxe::Mesh::CreateSphere(1);
	//j3->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//rob->attachPartsTree(4, "", j3);
	//Parts* a3 = new Parts();
	//a3->mesh_ = dxe::Mesh::CreateCylinder(0.5, 5);
	//a3->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//a3->ofs_pos_ += {0, 2.5, 0};
	//rob->attachPartsTree(4, "", a3);



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
	// ---- モジュールの構築情報をCSVから取得＆ツリー構造構築 ---- //
	std::string str_buf;
	std::string str_conma_buf;
	
	static std::string str[102][19];
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

	for (int i = 2; i < 102; i++) {		// ヘッダ2行は無視
		if (str[i][0] == "") { continue; }
		int attachi_id = stoi(str[i][0]);
		int id = stoi(str[i][1]);
		std::string name = str[i][2];
		tnl::Vector3 pos = { stof(str[i][3]), stof(str[i][4]), stof(str[i][5]) };
		tnl::Vector3 rot_axis = { stof(str[i][6]), stof(str[i][7]), stof(str[i][8]) };
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis(rot_axis, tnl::ToRadian(stof(str[i][9])));
		tnl::Vector3 dirz = { stof(str[i][10]), stof(str[i][11]), stof(str[i][12]) };
		tnl::Vector3 dirx = { stof(str[i][13]), stof(str[i][14]), stof(str[i][15]) };
		Module* mod = Module::createModule(id, name, pos, rot_axis, rot, dirz, dirx);	// モジュール生成
		if (str[i][16] == "on") {
			// 座標系を表示するか
			float size = stof(str[i][17]);
			float length = stof(str[i][18]);
			mod->setAxisView(size, length);
		}
		rob->attachModuleTree(attachi_id, "", mod);			// アタッチ
	}
}

void Robot::getPartsDataCSV(Robot* rob, std::string csv_path) {
	// ---- パーツの構築情報をCSVから取得 ---- //


	// やり方が分からない.. 保留;





	// --- 今度はこれをアッタッチしたい --- 
	//Parts* box_s = new Parts();
	//box_s->mesh_ = dxe::Mesh::CreateBox(3);
	//box_s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//box->_parts.push_back(box_s);

	//rob->attachModule(rob, box);

	//// test : 更に四角形をアタッチする
	//Module* box2 = Module::createModule(3, "box2", { 0, 15, 0 }, { 0, 1, 0 },
	//	tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-45)));
	//box2->setAxisView(0.3, 5);
	//Parts* box_2s = new Parts();
	//box_2s->mesh_ = dxe::Mesh::CreateBox(3);
	//box_2s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	//box2->_parts.push_back(box_2s);

	//box->attachModule(box, box2);

}

void Robot::getIKSetDataCSV(Robot* rob, std::string csv_path) {
	// ---- 逆運動学計算の設定をCSVから取得&特定モジュールに情報格納 ---- //
	std::string str_buf;
	std::string str_conma_buf;
	ik_st tmp_ikst;
	std::vector<ik_st> tmp_iksts;

	static std::string str[102][8];
	int i = 0;
	int j = 0;

	// --- csvを開く ---
	std::ifstream ifs(csv_path);
	if (!ifs) {
		printf("error! File can't opened");
	}
	while (std::getline(ifs, str_buf)) {
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

	for (int i = 2; i < 102; i++) {		// ヘッダ2行は無視
		if (str[i][0] == "") { continue; }
		int attachi_id = stoi(str[i][0]);
		std::string attachi_name = str[i][1];
		int cont_id = stoi(str[i][2]);
		std::string cont_name = str[i][3];
		int target_id = stoi(str[i][4]);
		std::string target_name = str[i][5];
		int type = (int)std::atoi(str[i][6].c_str());
		float kp = stof(str[i][7]);
		Module* cont_mod = new Module;
		cont_mod = rob->getModulePointerTree(cont_mod, cont_id, cont_name);
		Module* target_mod = new Module;
		target_mod = rob->getModulePointerTree(target_mod, target_id, target_name);
		tmp_ikst = { attachi_id, attachi_name, type, kp, target_mod, cont_mod };
		tmp_iksts.push_back(tmp_ikst);
	}
	rob->attachIKstTree(tmp_iksts);
}


