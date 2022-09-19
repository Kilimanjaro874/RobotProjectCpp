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
	tnl::Vector3 pos_next;	// 次モジュールの位置を計算する
	float link_length;		// 本モジュールのリンク長さ
	tnl::Vector3 link_dir;			// リンクの方向
	tnl::Quaternion rot_;	// LDK：本座標の局所的な回転クォータニオン
	tnl::Quaternion rot_sum_;	// 初期姿勢からの合計の回転を表すクォータニオン(パーツの位置・姿勢更新に使用)
	tnl::Vector3 rotAi_;	// モジュールの回転軸ベクトル(本モジュールΣi原点に対して立てる運用とする)
	// ---- 逆運動学計算に必要な変数 ----
	float kp_;				// 局所逆運動学の微小角度計算に用いる比例定数kp
	
	// ---- シェーダー系 ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- 順運動学を解く関数:LDK(内部でLIKも同時実行) -----
	virtual void localDirectKinematics(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back, 
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// --- 逆運動学を解く関数:LIK ----
	virtual tnl::Quaternion localInverseKinematics(float delta_time, const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	
	// ---- 局所順運動学計算：逆運動学計算も内部で実施 ---- //
	virtual void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// ---- 局所順運動学計算：逆運動学計算無し
	virtual void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
};

