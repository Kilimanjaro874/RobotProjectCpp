#pragma once
#include "gm_module.h"

class Agent : Module{
	// ----- ���W���[���𓝊�����G�[�W�F���g���N���X���쐬(Module�@�\�͌p��) ----- //
public:
	Agent() {};
	virtual ~Agent() {
		for (auto mod : modules_) delete mod;
	}
	// ---- ���W���[���Ǘ� ---- //
	std::vector<Module*> modules_;

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ----- ���^���w�������֐��FLDK for Agent�� ----- //
	void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) override;
	void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) override;
};