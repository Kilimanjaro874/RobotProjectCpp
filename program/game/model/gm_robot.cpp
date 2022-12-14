#include "gm_robot.h"
#include "gm_weapon.h"
#include <string>
#include <fstream>
#include <sstream>

Robot* Robot::Create(const tnl::Vector3 pos, const tnl::Quaternion rot) {
	// ----- 位置 pos, 姿勢 rot へロボットを生成 ----- //
	Robot* rob = new Robot();
	// ---- rob : プレイヤーの操作を受け付ける。そのための初期化実施 ---- //
	rob->init(rob, 1, "rob_ref_coord", pos, { 0, 1, 0 }, rot);
	//rob->setAxisView(0.2, 2.0);
	rob->getModuleDataCSV(rob, "RP_ModuleSet002.csv");
	rob->getIKSetDataCSV(rob, "RP_ModuleIKSet002.csv");
	
	// ---- parts取付機能実装前：こちらでパーツ生成＆アッタッチ ---- //
	std::string normal_texture = "graphics/test.jpg";
	
	// ---- LBXX : 100 腰のメッシュ生成 ---- //
	int const sLB = 2;
	Parts* LB0X_J[sLB];
	int sLBid[sLB] = { 102, 102 };
	LB0X_J[0] = new Parts();
	LB0X_J[0]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2);
	LB0X_J[0]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	LB0X_J[1] = new Parts();
	LB0X_J[1]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.5);
	LB0X_J[1]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	LB0X_J[1]->ofs_pos_ += {0, -1, 0};
	LB0X_J[1]->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	// --- attach --- //
	for (int i = 0; i < sLB; i++) {
		rob->attachPartsTree(sLBid[i], "", LB0X_J[i]);
	}

	// ---- BDXX : 200 体のメッシュ生成 ---- //
	int const sBD = 6;
	Parts* BD0X_J[sBD];
	int sBDid = 202;
	BD0X_J[0] = new Parts();
	BD0X_J[0]->mesh_ = dxe::Mesh::CreateSphere(0.3);
	BD0X_J[0]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[1] = new Parts();
	BD0X_J[1]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
	BD0X_J[1]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[1]->ofs_pos_ += {0, 0.25, 0};
	BD0X_J[2] = new Parts();
	BD0X_J[2]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
	BD0X_J[2]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[2]->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(45));
	BD0X_J[2]->ofs_pos_ += {-0.75, 1.25, 0};
	BD0X_J[3] = new Parts();
	BD0X_J[3]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 2.1213);
	BD0X_J[3]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[3]->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(-45));
	BD0X_J[3]->ofs_pos_ += {0.75, 1.25, 0};
	BD0X_J[4] = new Parts();
	BD0X_J[4]->mesh_ = dxe::Mesh::CreateCylinder(0.2, 4.0);
	BD0X_J[4]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[4]->ofs_rot_ = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(90));
	BD0X_J[4]->ofs_pos_ += {0, 2, 0};
	BD0X_J[5] = new Parts();
	BD0X_J[5]->mesh_ = dxe::Mesh::CreateSphere(0.75);
	BD0X_J[5]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	BD0X_J[5]->ofs_pos_ += {0, 1.2, 0};
	for (int i = 0; i < sBD; i++) {
		rob->attachPartsTree(sBDid, "", BD0X_J[i]);
	}
	
	// ---- RArmXX: 300 右腕のメッシュ生成 ---- //
	int const sRA = 3;
	Parts* RA0X_J[sRA];
	Parts* RA0X_A[sRA];
	int sRAid[sRA] = { 301, 303, 306 };
	float sRAsize[sRA] = { 2, 2, 1 };
	for (int i = 0; i < sRA; i++) {
		RA0X_J[i] = new Parts();
		RA0X_J[i]->mesh_ = dxe::Mesh::CreateSphere(0.3);
		RA0X_J[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		rob->attachPartsTree(sRAid[i], "", RA0X_J[i]);
		RA0X_A[i] = new Parts();
		RA0X_A[i]->mesh_ = dxe::Mesh::CreateCylinder(0.2, sRAsize[i]);
		RA0X_A[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		RA0X_A[i]->ofs_pos_ += {0, - sRAsize[i]/2, 0};
		rob->attachPartsTree(sRAid[i], "", RA0X_A[i]);
	}

	// ---- LArmXX: 400 左腕のメッシュ生成 ---- //
	int const sLA = 3;
	Parts* LA0X_J[sLA];
	Parts* LA0X_A[sLA];
	int sLAid[sLA] = { 401, 403, 406 };
	float sLAsize[sLA] = { 2, 2, 1 };
	for (int i = 0; i < sLA; i++) {
		LA0X_J[i] = new Parts();
		LA0X_J[i]->mesh_ = dxe::Mesh::CreateSphere(0.3);
		LA0X_J[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		rob->attachPartsTree(sLAid[i], "", LA0X_J[i]);
		LA0X_A[i] = new Parts();
		LA0X_A[i]->mesh_ = dxe::Mesh::CreateCylinder(0.2, sLAsize[i]);
		LA0X_A[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		LA0X_A[i]->ofs_pos_ += {0, -sLAsize[i] / 2, 0};
		rob->attachPartsTree(sLAid[i], "", LA0X_A[i]);
	}

	// ---- HDXX: 500 頭のメッシュ生成 ---- //
	Parts* Neck;
	Parts* Head;
	Neck = new Parts();
	Neck->mesh_ = dxe::Mesh::CreateCylinder(0.2, 0.5);
	Neck->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	Neck->ofs_pos_ += {0, 0.25, 0};
	rob->attachPartsTree(501, "", Neck);
	Head = new Parts();
	Head->mesh_ = dxe::Mesh::CreateBox(0.5);
	Head->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	rob->attachPartsTree(502, "", Head);

	// ---- RLXX: 600 右脚のメッシュ生成 ---- //
	int const sRL = 3;
	Parts* RL0X_J[sRL];
	Parts* RL0X_A[sRL];
	Parts* RLTOE;
	int sRLid[sRL] = { 601, 602, 605 };
	float sRLsize[sRL] = { 2.5, 2.5, 1 };
	for (int i = 0; i < sRL; i++) {
		RL0X_J[i] = new Parts();
		RL0X_J[i]->mesh_ = dxe::Mesh::CreateSphere(0.4);
		RL0X_J[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		rob->attachPartsTree(sRLid[i], "", RL0X_J[i]);
		RL0X_A[i] = new Parts();
		RL0X_A[i]->mesh_ = dxe::Mesh::CreateCylinder(0.3, sRLsize[i]);
		RL0X_A[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		RL0X_A[i]->ofs_pos_ += {0, -sRLsize[i] / 2, 0};
		rob->attachPartsTree(sRLid[i], "", RL0X_A[i]);
	}
	RLTOE = new Parts();
	RLTOE->mesh_ = dxe::Mesh::CreateBox(0.5);
	RLTOE->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	RLTOE->ofs_pos_ += {0, -0.75, 0.5};
	rob->attachPartsTree(605, "", RLTOE);


	// ---- LLXX: 700 右脚のメッシュ生成 ---- //
	int const sLL = 3;
	Parts* LL0X_J[sLL];
	Parts* LL0X_A[sLL];
	Parts* LLTOE;
	int sLLid[sLL] = { 701, 702, 705 };
	float sLLsize[sLL] = { 2.5, 2.5, 1 };
	for (int i = 0; i < sLL; i++) {
		LL0X_J[i] = new Parts();
		LL0X_J[i]->mesh_ = dxe::Mesh::CreateSphere(0.4);
		LL0X_J[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		rob->attachPartsTree(sLLid[i], "", LL0X_J[i]);
		LL0X_A[i] = new Parts();
		LL0X_A[i]->mesh_ = dxe::Mesh::CreateCylinder(0.3, sLLsize[i]);
		LL0X_A[i]->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
		LL0X_A[i]->ofs_pos_ += {0, -sLLsize[i] / 2, 0};
		rob->attachPartsTree(sLLid[i], "", LL0X_A[i]);
	}
	LLTOE = new Parts();
	LLTOE->mesh_ = dxe::Mesh::CreateBox(0.5);
	LLTOE->mesh_->setTexture(dxe::Texture::CreateFromFile(normal_texture));
	LLTOE->ofs_pos_ += {0, -0.75, 0.5};
	rob->attachPartsTree(705, "", LLTOE);

	// ---- RWXX : 800 右手武器生成 ---- //
	Module* r_arm = rob->getModulePointerTree(rob, 307, "RA04_EE");	// 右手武器をアタッチするモジュールポインタ取得
	Weapon* R_RIFLE = Weapon::createWeaponMod(800, "RW_01", 1.0/15, 100, Weapon::normal, r_arm->_pos, r_arm->_rot_axis, r_arm->_rot, r_arm->_dir_z, r_arm->_dir_x);
	//R_RIFLE->setAxisView(0.5, 1);
	rob->attachModuleTree(r_arm->_id, r_arm->_name, R_RIFLE);
	// ---- LWXX : 850 左手武器生成 ----- //
	Module* l_arm = rob->getModulePointerTree(rob, 407, "LA04_EE");	// 右手武器をアタッチするモジュールポインタ取得
	Weapon* L_RIFLE = Weapon::createWeaponMod(850, "LW_01", 1.0/2, 100, Weapon::rifle, l_arm->_pos, l_arm->_rot_axis, l_arm->_rot, l_arm->_dir_z, l_arm->_dir_x);
	//L_RIFLE->setAxisView(0.5, 1);
	rob->attachModuleTree(l_arm->_id, l_arm->_name, L_RIFLE);

	//Module* mod = Module::createModule(id, name, pos, rot_axis, rot, dirz, dirx);	// モジュール生成
	//if (str[i][16] == "on") {
	//	// 座標系を表示するか
	//	float size = stof(str[i][17]);
	//	float length = stof(str[i][18]);
	//	mod->setAxisView(size, length);
	//}
	//rob->attachModuleTree(attachi_id, "", mod);			// アタッチ
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

void Robot::move(float delta_time, const tnl::Vector3& pow,  const tnl::Quaternion& rot_move, const tnl::Vector3& gravity) {
	// ---- ロボットに加速度を与える：値に応じて座標更新 ---- //
	if (_dk_input.size() == 0) {
		printf("エラー：プレイヤー操作受付のための_dk_inputを初期化してください.");
		return;
	}
	acc_ = pow / mass;
	DrawStringEx(50, 120, -1, "acc x=%f, y=%f z=%f", acc_.x, acc_.y, acc_.z);
	vel_ += acc_ * delta_time + gravity * delta_time;
	
	// ---- 当たり判定や着地処理 ---- //
	// 1. 着地処理 
	if (_pos.y <= 0) { _pos.y = 0; }

	_dk_input[0]._dir = _pos + vel_;
	_dk_input[0]._length = 1;
	_dk_input[0]._rot_sum = rot_move;
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


