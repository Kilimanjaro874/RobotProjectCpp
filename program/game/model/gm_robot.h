#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	Robot() {};
	~Robot() {
		for (int n = 0; n < modules_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
	}
	// ----- ���{�b�g�̍\�� ----- //
	enum {
		e_lower_back_,
		e_body_,
		//e_r_arm,
		e_modules_max
	};
	// ----- ���[���h���W���猩���{���{�b�g��DK�p�����[�^ ----- //
	std::vector<dk_setting> rob_dk_s_v_;
	// ----- Render & DK & IK ------ //
	std::vector<std::vector<Module*>> modules_;

	// ----- �����o�֐� ------ //
	static Robot* Create(const tnl::Vector3& pos, const tnl::Quaternion& rot);

	// ---- mode01 : TPS�ˌ����[�h ---- //
	void mode01_init(float delta_time);
	void mode01_update(float delta_time);
	// --- �L�����R�� --- //
	void move(float delta_time);
};