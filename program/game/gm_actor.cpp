#include <fstream>
#include <sstream>
#include "gm_actor.h"

void tol::Actor::update(float delta_time) {
	Object::update(delta_time);
}

void tol::Actor::render(dxe::Camera* camera) {
	Object::render(camera);
}

std::shared_ptr<tol::Actor> tol::Actor::Create(std::shared_ptr<AssemRepo> assem_repo, std::string csv_path) {
	std::shared_ptr<Actor> act = std::make_unique<Actor>(Actor(0, "root"));
	act->coordinate_.setCoordinate();
	act->assemble_ = assem_repo->getAssemble(200, "", true, 1.0);
	act->getObjectDataCSV(assem_repo, csv_path);
	// -- kinematics setting (test) -- //
	act->kinematics_ = std::make_unique<Kinematics>(Kinematics());
	act->kinematics_->init(act);
	return act;
}

void tol::Actor::getObjectDataCSV(std::shared_ptr<AssemRepo> assem_repo, std::string csv_path) {
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
		int id				= stoi(c[0]);
		std::string name	= c[1];
		int parent_id		= stoi(c[2]);
		std::string parent_name = c[3];
		int assem_id		= stoi(c[20]);
		std::string assem_name	= c[21];
		
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
		obj->coordinate_.setCoordinate(
			pos,
			tnl::Quaternion::RotationAxis(axis, tnl::ToRadian(deg)),
			dirx, diry, dirz
			);
		obj->coordinate_.setViewCoordinate(1.1, 0.02);
		obj->assemble_ = assem_repo->getAssemble(assem_id, assem_name, true, a_size);
		obj->assemble_->setOffset_pos(a_offset_pos);
		obj->assemble_->setRot(tnl::Quaternion::RotationAxis(a_rot_axis, tnl::ToRadian(a_deg)));
		// -- register Actor class -- //
		std::shared_ptr<Object> parent = this->getObjectTree(parent_id, parent_name);
		if (parent != nullptr) {
			parent->setChild(obj);
			obj->setParent(parent);
		}
		// -- create kinematics -- //
		obj->kinematics_ = std::make_unique<Kinematics >(Kinematics());
		obj->kinematics_->init(obj);
	}
}

