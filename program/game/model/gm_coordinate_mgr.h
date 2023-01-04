#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "gm_coordinate.h"

class CoordinateMgr : public Coordinate {
public:
	CoordinateMgr() { init(); };
	~CoordinateMgr() {};
	enum class co_type{
		normal, target, object, end
	};
	enum class view_param {
		pos, rot_deg,  
	};
	struct coord_id_name_ik_st_ {
		int id_;
		std::string name_;
		ik_st_ attachi_ik_st;
	};

private:
	std::vector<std::vector<std::vector<Coordinate*>>> hierarchy_v_;

public:
	void init() override;
	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void registrateOrigine(Coordinate* origine, co_type type);
	bool registrateCoordinate(int parent_id, std::string parent_name, Coordinate* coord, co_type type, attach_type = attach_type::absolute);
	bool registrateIKCoordinate(const std::vector<coord_id_name_ik_st_>* c_ik_v);
	void viewCoordinateState(co_type type, view_param param);
	Coordinate* getRegistratedCoordinate(int id, std::string name, co_type type);
private:
	bool attachIK_st(const coord_id_name_ik_st_* c_ik_v);
};