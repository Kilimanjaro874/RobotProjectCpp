#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../model/gm_parts.h"
#include "gm_coordinate.h"

class Module : public Coordinate {
public:
	Module() {};
	~Module() {
		for (auto p : parts_) delete p;
	}

private:
	// ----- General ----- //

	// ----- Render ----- //
	std::vector<Parts*> parts_;
	bool _is_render = true;

	// ----- Forward Kinematics ----- //

public:
	void init() override;
	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	void setParts(Parts* p) { parts_.push_back(p); }
};