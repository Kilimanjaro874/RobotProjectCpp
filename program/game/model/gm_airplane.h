#pragma once
#include "gm_model.h"

class MdlAirPlane : public Model {
public :
	enum {
		e_body,
		e_l_front_wing,
		e_r_front_wing,
		e_l_back_wing,
		e_r_back_wing,
		e_t_back_wing,
		e_propeller,
		e_gyro,
		e_parts_max
	};

	float propeller_angle_ = 0;

	void update( float delta_time ) override;
	static MdlAirPlane* Create();
};
