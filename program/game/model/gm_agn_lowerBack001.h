#pragma once
#include "gm_agent.h"

class Agn_lowerBack001 : public Agent {
public:
	enum {
		e_bb_y,		// ”wœy²
		e_bb_x,		// ”wœx²
		e_bb_z,		// ”wœz²
		e_modules_max	// ‚¨‚í‚è
	};

	float backBone_length_ = 20;
	float hipJoint_length_ = 8;

	static Agn_lowerBack001* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	void mode01_init(const std::vector<FaceVec*> targets) override;	// mode01 : ƒ^[ƒQƒbƒg‚ÉŒü‚©‚Á‚ÄƒGƒCƒ€“®ì
	void mode01_update(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back, std::vector<FaceVec*> targets) override;
	void mode02_init(FaceVec& target) override;
	void mode02_update()override;

};