#include "gm_actor.h"
#include <fstream>
#include <sstream>
#include "gm_coordinate.h"
#include "gm_assemble.h"
#include "gm_kinematics.h"

void tol::Actor::init() {
	__super::init();
}

void tol::Actor::update(float delta_time) {
	__super::update(delta_time);
}

void tol::Actor::render(dxe::Camera* camera) {
	__super::render(camera);
}

std::shared_ptr<tol::Actor> tol::Actor::Create(std::shared_ptr<AssemRepo> a_repo, std::string csv_path) {
	// --- Actor init --- //
	std::shared_ptr<Actor> act = std::make_unique<Actor>(Actor(0, "root"));

	act->init();	// generate empty classes 
	//act->assemble_ = a_repo->CopyAssemble(200, "", true, 1.0);
	act->getObjectDataCSV(a_repo, csv_path);
	return act;
}

void tol::Actor::getObjectDataCSV(std::shared_ptr<AssemRepo> a_repo, std::string csv_path) {
	std::string str_buf;
	std::string str_conma_buf;
	static std::string str[103][30];
	// open csv
	int i = 0, j = 0;
	std::ifstream ifs(csv_path);
	if (!ifs) {
		printf("error! File can't opened");
		return;
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
	for (int i = 3; i < 103; i++) {		// ignore headers
		if (str[i][0] == "") {
			continue;
		}
		auto c = str[i];
		// -- general -- //
		int id = stoi(c[0]);
		std::string name = c[1];
		int parent_id = stoi(c[2]);
		std::string parent_name = c[3];
		int assem_id = stoi(c[20]);
		std::string assem_name = c[21];

		// -- vector, quaternion -- //
		// make : return vector func.
		auto vec = [](std::string x, std::string y, std::string z)-> tnl::Vector3 {
			return { stof(x), stof(y), stof(z) };
		};
		// info. : for object coorinate
		tnl::Vector3 pos = vec(c[4], c[5], c[6]);
		tnl::Vector3 axis = vec(c[7], c[8], c[9]);
		float deg = stof(c[10]);
		tnl::Vector3 dirx = vec(c[11], c[12], c[13]);
		tnl::Vector3 diry = vec(c[14], c[15], c[16]);
		tnl::Vector3 dirz = vec(c[17], c[18], c[19]);
		// info . : for object assemble
		tnl::Vector3 a_offset_pos = vec(c[22], c[23], c[24]);
		tnl::Vector3 a_rot_axis = vec(c[25], c[26], c[27]);
		float a_deg = stof(c[28]);
		float a_size = stof(c[29]);
		// -- create objects -- //
		std::shared_ptr<Object> obj = std::make_unique<Object>(Object(id, name));
		// - set parent - //
		std::shared_ptr<Object> parent = this->getObjectTree(parent_id, parent_name);
		if (parent != nullptr) {
			parent->setChild(obj);
			obj->setParent(parent);
		}
		// - create classes - //
		std::shared_ptr<Coordinate> cod = std::make_unique<Coordinate>(Coordinate());
		obj->setCoordinate(cod);
		cod->init(
			pos,
			tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(deg)),
			dirx,
			diry,
			dirz
		);
		std::shared_ptr<Assemble> assem = std::make_unique<Assemble>(Assemble());
		obj->setAssemble(assem);
		assem = a_repo->CopyAssemble(assem_id, assem_name, true, a_size);
		assem->setOffsetPos(a_offset_pos);
		assem->setOffsetRot(tnl::Quaternion::RotationAxis(a_rot_axis, tnl::ToRadian(a_deg)));
		assem->setCoordinateView(obj, 1.5, 0.05);
		std::shared_ptr<Kinematics> kinematics = std::make_unique<Kinematics>(Kinematics());
		obj->setKinematics(kinematics);
		if (parent) {
			kinematics->init(parent, obj);
		}
		obj->init(cod, assem, kinematics);
	}
}