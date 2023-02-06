#include "gm_actor.h"
#include <fstream>
#include <sstream>
#include "gm_coordinate.h"
#include "gm_assemble.h"
#include "gm_kinematics.h"
#include "gm_inverse_kinematics.h"

void tol::Actor::init() {
	__super::init();
}

void tol::Actor::update(float delta_time) {
	__super::update(delta_time);
}

void tol::Actor::render(dxe::Camera* camera) {
	__super::render(camera);
}

std::shared_ptr<tol::Actor> tol::Actor::Create(std::shared_ptr<AssemRepo> a_repo, std::string csv_path, std::string ik_csv_path) {
	// --- Actor init --- //
	std::shared_ptr<Actor> act = std::make_unique<Actor>(Actor(0, "root"));

	act->init();	// generate empty classes 
	//act->assemble_ = a_repo->CopyAssemble(200, "", true, 1.0);
	if (csv_path != "") {
		act->getObjectDataCSV(a_repo, csv_path);
	}
	if (ik_csv_path != "") {
		act->getIKsettingDataCSV(ik_csv_path);
	}
	//act->getIKsettingDataCSV()
	return act;
}

void tol::Actor::pidVellContUpdate(float delta_time, const tnl::Vector3& dir) {
	if (pid_vel_cont_) {
		if (ph_handler_) {
			auto cod = getCoordinate();
			// vel_dir : Convert input to local coordinate.
			tnl::Vector3 vel_dir = tnl::Vector3::TransformCoord(dir, cod->getRot());
			tnl::Vector3 current_velocity = ph_handler_->getVelocity();
			tnl::Vector3 local_force = pid_vel_cont_->update(delta_time, vel_dir, current_velocity);
			ph_handler_->update(delta_time, shared_from_this(), local_force);
			tnl::Vector3 new_velocity = ph_handler_->getVelocity();
			Translate(new_velocity, false);			// move
		}
	}
}

void tol::Actor::pidPosContUpdate(float delta_time, const tnl::Vector3& target_pos) {
	if (pid_pos_cont_) {
		if (ph_handler_) {
			auto cod = getCoordinate();
			tnl::Vector3 current_pos = cod->getPos();
			tnl::Vector3 force = pid_pos_cont_->update(delta_time, target_pos, current_pos);
			ph_handler_->update(delta_time, shared_from_this(), force);
			tnl::Vector3 new_velocity = ph_handler_->getVelocity();
			Translate(new_velocity, false);
		}
	}
}

void tol::Actor::pidRotContUpdate(float delta_time, const tnl::Vector3& look_ref_pos)
{
	if (pid_rot_cont_) {
		if (ph_handler_) {
			auto cod = getCoordinate();
			tnl::Vector3 current_dir = cod->getDirZ();
			tnl::Vector3 look_dir_pos_from_cod = look_ref_pos - cod->getPos();
			float torque = pid_rot_cont_->update(delta_time, look_dir_pos_from_cod, current_dir);
			ph_handler_->update(delta_time, shared_from_this(), { 0, 0, 0 }, torque);
			float ang_vel = ph_handler_->getRotVelocity();
			Rotation(tnl::Quaternion::RotationAxis({ 0, 1, 0 }, ang_vel), true);


		}
	}
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
		assem->setCoordinateView(obj, 1.5f, 0.05f);
		std::shared_ptr<Kinematics> kinematics = std::make_unique<Kinematics>(Kinematics());
		obj->setKinematics(kinematics);
		if (parent) {
			kinematics->init(parent, obj);
		}
		obj->init(cod, assem, kinematics);
	}
}

void tol::Actor::getIKsettingDataCSV(std::string csv_path) {
	std::string str_buf;
	std::string str_conma_buf;
	static std::string str[103][13];
	// --- open csv --- //
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
	for (int i = 3; i < 103; i++) {
		if (str[i][0] == "") {
			continue;
		}
		auto c = str[i];
		// --- get data --- // 
		int ik_id = stoi(c[0]);
		std::string ik_name = c[1];
		int set_obj_id = stoi(c[2]);
		std::string set_obj_name = c[3];
		int ik_obj_id = stoi(c[4]);
		std::string ik_obj_name = c[5];
		int ik_tar_id = stoi(c[6]);
		std::string ik_tar_name = c[7];
		int ik_type = stoi(c[8]);
		float kp = stof(c[9]);
		bool is_rotatable_axis[static_cast<int>(Coordinate::coordinate::end)];
		auto isrot = [](std::string s)-> bool {
			if (s == "on") { return true; }
			return false;
		};
		is_rotatable_axis[static_cast<int>(Coordinate::coordinate::x)] = isrot(c[10]);
		is_rotatable_axis[static_cast<int>(Coordinate::coordinate::y)] = isrot(c[11]);
		is_rotatable_axis[static_cast<int>(Coordinate::coordinate::z)] = isrot(c[12]);

		// --- check: already have InvKinematics component --- //
		std::shared_ptr<Object> attach_obj = this->getObjectTree(set_obj_id, set_obj_name);
		if (!attach_obj) { continue;}
		std::shared_ptr<Kinematics> attach_kinematics = attach_obj->getKinematics();
		if (!attach_kinematics) { continue; }
		std::shared_ptr<InvKinematics> tmp_ik_component = attach_kinematics->getInvKinematics();
		if (!tmp_ik_component) {
			// --- create InvKinametics Class --- //
			std::shared_ptr<Object> ik_obj = this->getObjectTree(ik_obj_id, ik_obj_name);
			std::shared_ptr<Object> ik_tar = this->getObjectTree(ik_tar_id, ik_tar_name);
			tmp_ik_component = std::make_shared<InvKinematics>(InvKinematics());
			tmp_ik_component->init(ik_tar, ik_obj,
				ik_id, ik_name, static_cast<tol::InvKinematics::ik_type>(ik_type), kp,
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::x)],
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::y)],
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::z)]
			);
			attach_kinematics->setInvKinematics(tmp_ik_component);
		}
		else {
			// --- already have InvKinematics Class --- //
			std::shared_ptr<Object> ik_obj = this->getObjectTree(ik_obj_id, ik_obj_name);
			std::shared_ptr<Object> ik_tar = this->getObjectTree(ik_tar_id, ik_tar_name);
			tmp_ik_component->init(ik_tar, ik_obj,
				ik_id, ik_name, static_cast<tol::InvKinematics::ik_type>(ik_type), kp,
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::x)],
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::y)],
				is_rotatable_axis[static_cast<int>(Coordinate::coordinate::z)]
			);
		}
	}
}