#pragma once
#include "gm_module.h"

class Agent : Module{
	// ----- モジュールを統括するエージェント基底クラスを作成(Module機能は継承) ----- //
public:
	Agent() {};
	virtual ~Agent() {
		for (auto mod : modules_) delete mod;
	}
	// ---- モジュール管理 ---- //
	std::vector<Module*> modules_;

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ----- 順運動学を解く関数：LDK for Agent版 ----- //
	void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) override;
	void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) override;
};