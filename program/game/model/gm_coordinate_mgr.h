#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "gm_coordinate.h"

class CoordinateMgr : Coordinate {
public:
	CoordinateMgr() { init(); };
	~CoordinateMgr() {};
	enum class co_type{
		normal, target, object, end
	};

private:
	std::vector<std::vector<std::vector<Coordinate*>>> hierarchy_v_;

public:
	void init() override;
	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void registrateOrigine(Coordinate* origine, co_type type);
	bool registrateCoordinate(int parent_id, std::string parent_name, Coordinate* coord, co_type type, attach_type = attach_type::absolute);
};
