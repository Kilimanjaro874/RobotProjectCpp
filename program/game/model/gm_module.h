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
	tnl::Vector3 pos_;			// エージェント座標xΣaiからの位置
	tnl::Vector3 pos_next;		// 次モジュール[i+1]の位置を計算＆格納
	float link_length;			// 本モジュールのリンク長さ
	tnl::Vector3 link_dir;		// 本モジュールから次のモジュールへの方向単位ベクトル
	tnl::Quaternion rot_;		// LDK：本座標の局所的な回転クォータニオン
	tnl::Quaternion rot_sum_;	// 初期姿勢からの合計の回転を表すクォータニオン(パーツの位置・姿勢更新に使用)：また、いずれ関節動作角度制限のための情報にもなるはず
	tnl::Vector3 rotAi_;		// モジュールの回転軸ベクトル(本モジュールxΣi原点に対して。本単位ベクトル周りに回転を与える)
	float kp_;					// 局所逆運動学の微小角度計算に用いる比例定数kp
	std::vector<float> kp_num_;			// モジュール微小角度計算比例定数(複数の場合あり) 
	std::vector<tnl::Vector3> pos_r_;	// モジュール手先目標位置(複数の場合あり)
	std::vector<tnl::Vector3> pos_e_;	// pos_rに追従させたい手先参照位置(複数の場合あり)
	
	// ---- パーツ管理 ----
	std::vector<Parts*> parts_;

	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	// ---- 順運動学を解く関数:LDK(内部でLIKも同時実行) -----

	// --- 逆運動学を解く関数:LIK ----
	virtual tnl::Quaternion localInverseKinematics(float delta_time);
	
	// ---- 局所順運動学計算：逆運動学計算も内部で実施 ---- //
	virtual void localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	// ---- 局所順運動学計算：逆運動学計算無し
	virtual void localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	// ---- pr, peのベクター更新 ---- //
	//virtual void setTarget_EE(std::vector<tnl::Vector3> pos_r, std::vector<tnl::Vector3> pos_e);

};

