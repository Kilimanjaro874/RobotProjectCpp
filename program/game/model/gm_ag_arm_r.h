#pragma once
#include "gm_module.h"

class MdlArm_r : public Module {
	// ----- ロボット：右腕を想定:モジュールをまとめあげるエージェントとして動作 ----- //
public:
	enum {
		e_shoulder,		// 肩
		e_models_max	// おわり
	};
	std::vector<Module*>modules_;	// モジュールのベクター型

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ---- 各モジュールのLocalDirectKinematicsをエージェントとして実施 ---- //
	void calcLDK(const tnl::Quaternion& q_back, const tnl::Vector3& l_back);
	static MdlArm_r* Create();
};