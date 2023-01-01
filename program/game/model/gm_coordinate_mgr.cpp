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