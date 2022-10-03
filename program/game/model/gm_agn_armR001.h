#pragma once
#include "gm_agent.h"

class Agn_armR001 : public Agent {
public:
	enum {
		e_sho_x,		// 肩：x軸モジュール
		e_sho_y,		// 肩：y軸
		e_arm_x,		// 腕
		e_arm_z,
		e_wrist_z,
		e_wrist_x,		// 手首
		e_wrist_z2,
		e_modules_max	// おわり
	};

	static Agn_armR001* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	
	void aimTarget_initialize(FaceVec& target, FaceVec& cnt_obj, FaceVec& elbow);
	void aimTarget_update(float delta_time, const FaceVec& target);
};