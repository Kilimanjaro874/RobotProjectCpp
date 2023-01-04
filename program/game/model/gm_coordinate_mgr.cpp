#include "gm_coordinate_mgr.h"
#include <algorithm>

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
			hierarchy_v_[type_no][col][itr_index]->setChildAndDKInit(coord, a_type);
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