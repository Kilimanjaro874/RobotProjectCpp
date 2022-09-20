#include "gm_agent.h"

void Agent::update(float delta_time)  {
	for (auto mod : modules_) {
		mod->update(delta_time);
	}
}

void Agent::render(dxe::Camera* camera) {
	for (auto mod : modules_) {
		mod->render(camera);
	}
}

void Agent::localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
	const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) {
	// ----- 外部からp_back, l_backを貰い、保持モジュール全てのLKD計算を実施する関数 ----- //
	// LIKも同時実行
	// ---- 自身のLDK実施 ---- //
	this->localDKwithLIK(delta_time, p_back, q_back, pos_e, pos_r);
	tnl::Vector3 tmp_p_back = this->pos_next;
	tnl::Quaternion tmp_q_back = this->rot_;

}