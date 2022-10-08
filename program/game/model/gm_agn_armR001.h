#pragma once
#include "gm_agent.h"

class Agn_armR001 : public Agent {
public:
	enum {
		e_sho_x,		// ���Fx�����W���[��
		e_sho_y,		// ���Fy��
		e_arm_x,		// �r
		e_arm_z,
		e_wrist_z,
		e_wrist_x,		// ���
		e_wrist_z2,
		e_modules_max	// �����
	};

	static Agn_armR001* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);

	void mode01_init(const std::vector<FaceVec*> targets) override;	// mode01 : �^�[�Q�b�g�Ɍ������ăG�C������
	void mode01_update(float delta_time) override;	
	void mode02_init(FaceVec& target) override;
	void mode02_update()override;
};