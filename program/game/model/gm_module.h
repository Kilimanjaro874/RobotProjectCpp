#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	// ----- Render ----- //
	std::vector<Parts*> parts_;
	bool is_render_ = true;		// 描画するか否か
	// ----- Parameters ----- //
	int id_;		// 参照用
	Module* back;				// 親モジュール
	std::vector<Module*> next;	// 子モジュール
	// ----- Direct Kinematics : DKに必要な変数 ----- //
	struct dk_setting {
		int id_;				// 本dk_settingを受け取るモジュールid_を指定
		tnl::Vector3 dir_r_n_;	// 次モジュールへの方向単位ベクトル
		float dir_r_length_;	// 次モジュールへの距離の大きさ格納
		tnl::Quaternion q_r_n_;	// 次モジュールへの回転量
	};
	std::vector<dk_setting> dk_s_v_;	// DKセッティングのベクトル型
	std::vector<dk_setting> in_dk_s_v_;	// DKセッティングの基準を作る
	tnl::Vector3 pos_o_;			// ワールド座標系の本モジュールiの原点位置
	tnl::Vector3 in_rot_axis_;		// 本モジュールiの回転軸単位ベクトル：本モジュール原点に立てる（初期値)
	tnl::Vector3 rot_axis_;			// 回転軸単位ベクトル
	tnl::Vector3 in_dir_z_;			// 本モジュールiのz軸方向単位ベクトル(初期値)
	tnl::Vector3 dir_z_;			// z軸方向単位ベクトル
	tnl::Vector3 in_dir_x_;			// 本モジュールiのx軸方向単位ベクトル(初期値)
	tnl::Vector3 dir_x_;			// x軸方向単位ベクトル
	tnl::Quaternion rot_tmp_;		// IK計算時、1フレーム間のみ有効なクォータニオン
	tnl::Quaternion rot_sum_;		// 初期状態から本モジュールiの回転量を表すクォータニオン

	// ---- Inverse Kinematics : IKに必要な変数 ----- //
	struct ik_setting {
		int ik_type_;			// IKで解くアルゴリズムの種類を格納
		float kp_;				// dthが過剰な回転とならないよう抑制する比例定数(0~1.0);
		Module* target_;	// 目標位置クラス
		Module* object_;	// 制御対象クラス
	};
	std::vector<ik_setting> ik_s_v_;

	enum ik_type {
		pos_to_pos,
		dirz_to_pos,
		dirx_to_pos,
		dirz_to_dirz,
		dirx_to_dirx,
		dirz_to_dirx,
		dirx_to_dirz
	};

	// ---- メンバ関数 ---- //
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	void InitParams(int id, tnl::Vector3 rot_axis, tnl::Quaternion rot_sum, 
		tnl::Vector3 dir_z = {0, 0, 1}, tnl::Vector3 dir_x = {1, 0, 0});
	void InitDK(const std::vector<dk_setting>& dks);
	void SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot);
	void DKwithIK(float delta_time, const std::vector<dk_setting>& dks);
	// test
	void AllInitDK(const Module* mod, const std::vector<dk_setting>& dks);
	void updateTree(const Module* mod, float delta_time);
	void renderTree(const Module* mod, dxe::Camera* camera);
	void AttachModule(Module* mod, Module* attach_mod, int id);
	void SetIKParams(Module* mod, int id, Module* target, Module* object,
		int ikType = pos_to_pos, float kp = 0.4);
	void AllupdateIK(const Module* mod, float delta_time);
};