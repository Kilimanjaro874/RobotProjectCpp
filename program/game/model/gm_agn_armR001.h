#pragma once
#include "gm_agent.h"

class Agn_armR001 : public Agent {
public:
	enum {
		e_sho_x,		// ¨Fx²W[
		e_sho_y,		// ¨Fy²
		e_arm_x,		// r
		e_arm_z,
		e_wrist_z,
		e_wrist_x,		// èñ
		e_wrist_z2,
		e_modules_max	// ¨íè
	};

	static Agn_armR001* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	
	void aimTarget_initialize(FaceVec& target, FaceVec& cnt_obj, FaceVec& elbow);
	void aimTarget_update(float delta_time, const FaceVec& target);
};