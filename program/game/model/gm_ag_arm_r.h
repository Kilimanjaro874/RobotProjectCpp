#pragma once
#include "gm_module.h"

class MdlArm_r : public Module {
	// ----- ���{�b�g�F�E�r��z��:���W���[�����܂Ƃ߂�����G�[�W�F���g�Ƃ��ē��� ----- //
public:
	enum {
		e_shoulder,		// ��
		e_models_max	// �����
	};
	std::vector<Module*>modules_;	// ���W���[���̃x�N�^�[�^

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ---- �e���W���[����LocalDirectKinematics���G�[�W�F���g�Ƃ��Ď��{ ---- //
	void calcLDK(const tnl::Quaternion& q_back, const tnl::Vector3& l_back);
	static MdlArm_r* Create();
};