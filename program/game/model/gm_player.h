#pragma once
#include "gm_module.h"
#include "gm_ag_arm_r.h"
class Player : public MdlArm_r{
public:
	
	enum {
		arm,
		e_modules_max
	};
	std::vector<MdlArm_r*>modules_;

	tnl::Vector3 forward_;
	tnl::Vector3 delta_pos;

	int tmpangle;

	void update(float delta_time) override;
	void render(dxe::Camera* camera);

	void move(float delta_time);
	static Player* Create();

	void calcLDK();
};