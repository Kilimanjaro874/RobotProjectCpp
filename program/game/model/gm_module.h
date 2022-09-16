#pragma once
#include "gm_parts.h"

class Module {
public:
	// ----- 順運動学計算機能を持たせたモデルのようなイメージ ----- //
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	// ---- 順運動学計算に必要な変数 ----
	tnl::Vector3 pos_;		// 座標xΣiからの位置
	tnl::Quaternion rot_;	// LDK：本座標の局所的な回転クォータニオン
	tnl::Vector3 xli_;		// LDK: 本モジュール回転により後軸へ与える平行移動量を格納
	tnl::Quaternion rot_sum_;	// 初期姿勢からの合計の回転を表すクォータニオン(パーツの位置・姿勢更新に使用)
	tnl::Vector3 rotAi_;	// モジュールの回転軸ベクトル(本モジュールΣi原点に対して立てる運用とする)
	
	// ---- シェーダー系 ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- 順運動学を解く関数 ----- 
	virtual void localDirectKinematics(const tnl::Quaternion& q_back, const tnl::Vector3& l_back);

};