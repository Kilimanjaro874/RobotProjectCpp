#include "gm_coordinate_mgr.h"
#include <algorithm>
#include <fstream>
#include <sstream>

void CoordinateMgr::init() {
	hierarchy_v_.resize(static_cast<int>(co_type::end));
	for (int i = 0; i < hierarchy_v_.size(); i++) {
		hierarchy_v_[i].resize(1);
	}
}

void CoordinateMgr::update(float delta_time) {
	for (int i = 0; i < hierarchy_v_.size(); i++) {
		for (int c = 0; c < hierarchy_v_[i].size(); c++) {
			for (int r = 0; r < hierarchy_v_[i][c].size(); r++) {
				if (hierarchy_v_[i][c][r] == nullptr) { continue; }
				hierarchy_v_[i][c][r]->update(delta_time);
			}
		}
	}
}

void CoordinateMgr::render(dxe::Camera* camera) {
	for (int i = 0; i < hierarchy_v_.size(); i++) {
		for (int c = 0; c < hierarchy_v_[i].size(); c++) {
			for (int r = 0; r < hierarchy_v_[i][c].size(); r++) {
				if (hierarchy_v_[i][c][r] == nullptr) { continue; }
				hierarchy_v_[i][c][r]->render(camera);
			}
		}
	}
}

void CoordinateMgr::registrateOrigine(Coordinate* origine, co_type type) {
	hierarchy_v_[static_cast<int>(type)][0].push_back(origine);
}

bool CoordinateMgr::registrateCoordinate(int parent_id, std::string parent_name, Coordinate* coord, 
	co_type type, attach_type a_type
) {
	int col = -1;
	int itr_index = -1;
	// ---- id or name search ---- //
	int type_no = static_cast<int>(type);
	for (int c = 0; c < hierarchy_v_[type_no].size(); c++) {
		auto itr = std::find_if(hierarchy_v_[type_no][c].begin(), hierarchy_v_[type_no][c].end(), [&](auto& c) {
			return (c->getId() == parent_id || c->getName() == parent_name);
			});
		if (itr != hierarchy_v_[type_no][c].end()) {
			col = c;
			int itr_index = std::distance (hierarchy_v_[type_no][c].begin(), itr);
			auto parent = hierarchy_v_[type_no][col][itr_index];
			parent->setChildAndDKInit(coord, a_type);
			coord->setParent(parent);
			if (hierarchy_v_[type_no][col].size() <= (col + 1)) { hierarchy_v_[type_no].resize(col + 2); }
			hierarchy_v_[type_no][col + 1].push_back(coord);

			return true;
		}
	}

	return false;
}

bool CoordinateMgr::registrateIKCoordinate(const std::vector<coord_id_name_ik_st_>* c_ik_v) {

	for (auto c_ik : *c_ik_v) {
		if (!attachIK_st(&c_ik)) {
			DrawStringEx(500, 10, -1, "Error: registrateIKCoordinate");
			return false;
		}
	}
	return true;
}

void CoordinateMgr::viewCoordinateState(co_type type, view_param param) {
	int count = 0;
	int col = 10;
	int row = 5;
	int col_quantity = 20;
	int row_quantity = 5;
	switch (param)
	{
	case CoordinateMgr::view_param::pos:
		for (int c = 0; c < hierarchy_v_[static_cast<int>(type)].size(); c++) {
			for (int r = 0; r < hierarchy_v_[static_cast<int>(type)][c].size(); r++) {
				auto tmp_h = hierarchy_v_[static_cast<int>(type)][c][r];
				tnl::Vector3 tmp = tmp_h->getPos();
				DrawStringEx(row, col, -1, "%s(id:%d) pos = (%5.2f, %5.2f, %5.2f)", tmp_h->getName().c_str(), tmp_h->getId(), tmp.x, tmp.y, tmp.z);
				col += col_quantity;
			}
			row += row_quantity;
		}
		break;
	case CoordinateMgr::view_param::rot_deg:

		break;
	default:
		break;
	}
}

Coordinate* CoordinateMgr::getRegistratedCoordinate(int id, std::string name, co_type type) {
	auto tmp = hierarchy_v_[static_cast<int>(type)];
	for (int c = 0; c < tmp.size(); c++) {
		auto itr = std::find_if(tmp[c].begin(), tmp[c].end(), [&](auto& c) {
			return (c->getId() == id || c->getName() == name);
			});
		if (itr != tmp[c].end()) {
			int itr_index = std::distance(tmp[c].begin(), itr);
			return tmp[c][itr_index];
		}
	}
	return nullptr;
}

/// <summary>
/// Read coordinates from CSV data & define DK (FK) construction 
/// (set parent-child relationship of coordinates)
/// CSV data -> limit[col:103, row:27]
/// </summary>
/// <param name="csv_path"> CSV for DK settings </param> 
void CoordinateMgr::getCoordinateDataCSV(std::string csv_path) {
	std::string str_buf;
	std::string str_conma_buf;
	static std::string str[103][32];

	// open the csv
	int i = 0, j = 0;
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

	for (int i = 3; i < 103; i++) {		// ignore headers
		if (str[i][0] == "") { continue; }
		auto c = str[i];
		// general
		int id = stoi(c[0]);
		std::string name = c[1];
		int parent_id = (c[2] != "")?  stoi(c[2]) : -1 ;					// -1 : origine flag
		std::string parent_name = (c[3] != "")? c[3] : "this_is_origine";	
		float deg = stof(c[10]);
		bool is_show_coordinate = (c[29] == "on") ? true : false;
		float size = 0, length = 0;
		if (is_show_coordinate) {
			size = stof(c[30]);
			length = stof(c[31]);
		}
		// vector
		auto vec = [](std::string x, std::string y, std::string z)-> tnl::Vector3 {
			return {stof(x), stof(y), stof(z)};
		};
		tnl::Vector3 pos, rotAxis;
		tnl::Vector3 vdr[6];
		pos = vec(c[4], c[5], c[6]);
		rotAxis = vec(c[7], c[8], c[9]);
		for (int k = 0; k < 6; k++) {
			int row = 11;
			int tmp = 3 * k;
			vdr[k] = vec(c[row + tmp], c[row + tmp + 1], c[row + tmp + 2]);
		}
		// create coordinate
		Coordinate* cod = new Coordinate();
		cod->setCoordinate(id, name,
			pos, vdr[0], vdr[1], vdr[2], vdr[3], vdr[4], vdr[5],
			tnl::Quaternion::RotationAxis(rotAxis, tnl::ToRadian(deg))
			);
		if (is_show_coordinate) { cod->setViewCoorinate(size, length); }
		// registrate coordinates
		if (parent_id == -1) {
			registrateOrigine(cod, CoordinateMgr::co_type::normal);
		}
		else {
			registrateCoordinate(parent_id, parent_name, cod, co_type::normal);
		}
	}
}

bool CoordinateMgr::attachIK_st(const coord_id_name_ik_st_* c_ik_v) {
	auto tmp = hierarchy_v_[static_cast<int>(co_type::normal)];
	for (int c = 0; c < tmp.size(); c++) {
		auto itr = std::find_if(tmp[c].begin(), tmp[c].end(), [&](auto& c) {
			return (c->getId() == c_ik_v->id_ || c->getName() == c_ik_v->name_);
			});
		if (itr != tmp[c].end()) {
			int itr_index = std::distance(tmp[c].begin(), itr);
			tmp[c][itr_index]->setIK_st(&c_ik_v->attachi_ik_st);
			return true;
		}
	}
	return false;
}