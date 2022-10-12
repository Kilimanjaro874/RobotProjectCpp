#pragma once
#include "gm_parts.h"
#include "gm_face_vector.h"

class Module {
public:
	Module() {};
	virtual ~Module() {

	}
	// ----- Render ----- //
	std::vector<Parts*> parts_;
	// ----- Parameters ----- //
	int id_;		// 参照用
	int hierarchy_;	// 階層参照用

	// ----- Direct Kinematics : DKに必要な変数 ----- //
	struct dk_setting {
		tnl::Vector3 dir_r_n_;	// 次モジュールへの方向単位ベクトル
		float dir_r_length_;	// 次モジュールへの距離の大きさ格納
	};
	std::vector<dk_setting> dk_s_v_;	// DKセッティングのベクトル型
	tnl::Vector3 pos_o_;			// ワールド座標系の本モジュールiの原点位置
	tnl::Vector3 in_rot_axis_;		// 本モジュールiの回転軸単位ベクトル：本モジュール原点に立てる（初期値)
	tnl::Vector3 in_dir_z_;			// 本モジュールiのz軸方向単位ベクトル(初期値)
	tnl::Vector3 in_dir_x_;			// 本モジュールiのx軸方向単位ベクトル(初期値)
	tnl::Quaternion rot_tmp_;		// IK計算時、1フレーム間のみ有効なクォータニオン
	tnl::Quaternion rot_sum_;		// 初期状態から本モジュールiの回転量を表すクォータニオン

	// ---- Inverse Kinematics : IKに必要な変数 ----- //
	struct ik_setting {
		int ik_type_;			// IKで解くアルゴリズムの種類を格納
		float kp_;				// dthが過剰な回転とならないよう抑制する比例定数(0~1.0);
		FaceVector* target_;	// 目標位置クラス
		FaceVector* object_;	// 制御対象クラス
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

	



};