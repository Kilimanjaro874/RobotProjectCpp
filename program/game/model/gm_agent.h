#pragma once
#include "gm_module.h"

class Agent : public Module {
public:
	Agent() {};
	~Agent() {
		for (auto mod : modules_) delete mod;
	}
	std::vector<Module*> modules_;		// モジュールを登録
	std::vector<FaceVec*> targets_;		// IKの目標位置・姿勢リスト
	std::vector<FaceVec*> cnt_objects_;	// IKの制御対象の位置・姿勢リスト

	// ---- エージェントで共通の作業を次々追加予定 ---- //
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	virtual void mode01_init(const std::vector<FaceVec*> targets) = 0;		// エージェントに与えるモーション１：初期化
	virtual void mode01_update(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back, std::vector<FaceVec*> targets);	// 〃 update
	virtual void mode02_init(FaceVec& target);
	virtual void mode02_update();

};