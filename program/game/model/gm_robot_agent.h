#pragma once 
#include "gm_model.h"
#include "gm_robot_module.h"

class MdlRobotAgent_Rarms : public MdlRobotModule {
	// ----- MdlRobotModuleをエージェントとして管理するクラス作成 ----- //
public:
	// ---- ひとまずロボットの右腕としで構成 ---- //
	enum {
		e_shoulder,		// 肩
		e_arm,			// 腕
		e_wrist,			// 手首
		e_models_max
	};

	tnl::Vector3 pos_;		// エージェント原点の位置ベクトルとして使用(pΣi基準)
	tnl::Quaternion rot_ ;	// エージェントの局所的な回転として使用(pΣi基準)
	tnl::Vector3 rotAi_;
	tnl::Vector3 posLi_;
	tnl::Vector3 oLi = { 0, 0, 0 };

	std::vector<MdlRobotModule*>modules_;	// モデルのベクトル型

	void update(float delta_time);
	void render(dxe::Camera* camera);
	static MdlRobotAgent_Rarms* Create();
	void directKinematics();
};