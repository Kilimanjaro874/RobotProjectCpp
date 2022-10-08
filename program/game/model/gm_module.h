#pragma once
#include "gm_parts.h"
#include "gm_faceVec.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	int id_;						// 参照用

	enum ik_type {
		pos_to_pos,
		dirz_to_pos,
		dirx_to_pos,
		dirz_to_dirz,
		dirx_to_dirx,
		dirz_to_dirx,
		dirx_to_dirz
	};
	// ----- Inverse Kinnematics : IKに必要な変数 ---- //
	struct ik_setting
	{
		int ik_type;		// IKで解くタイプ(1 ~ 5)
		float kp;			// dthが過剰な値とならないよう抑制する比例定数
		FaceVec* target;	// 目標位置・姿勢のポインタ
		FaceVec* object;	// 制御するオブジェクトの位置・姿勢のポインタ
	};
	std::vector<ik_setting> ik_settings_;

	// ---- Direct Kinematics : DKに必要な変数 ---- //
	tnl::Vector3 pos_o_;			// ワールド座標oΣからの本モジュールiの座標系原点位置
	tnl::Vector3 pos_o_next_;		// ワールド座標系：次モジュールの位置を格納
	tnl::Vector3 dir_z_;			// 本モジュールiからモジュールi+iへの方向単位ベクトルを定義(位置IK計算に使用)
	tnl::Vector3 init_dir_z_;		// 本モジュールiからモジュールi+iへの方向単位ベクトルを定義・初期状態(位置IK計算に使用)
	tnl::Vector3 dir_x_;			// 本モジュールi姿勢制御のための姿勢方向単位ベクトルを定義(姿勢IK計算に使用)
	tnl::Vector3 init_dir_x_;		// 本モジュールiの姿勢定義の為の単位ベクトルを定義・初期状態(位置IK計算に使用)
	tnl::Vector3 rot_axis_;			// モジュールiの回転軸単位ベクトル：本モジュール原点に立てる事とする
	tnl::Vector3 init_rot_axis_;	// 本モジュールiの回転軸単位ベクトル・初期状態を定義
	float link_length_;				// dir_z_の大きさを格納：モジュールi原点からモジュールi+iまでの長さを定義
	tnl::Quaternion rot_tmp_;		// IK計算時、oΣ基準に記述された、1フレーム間で本モジュールが取る一時的な回転量を格納
	tnl::Quaternion rot_sum_;		// 本モジュールが初期姿勢からどれだけ回転したかを格納しておく。
	// ---- Render ----- //
	std::vector<Parts*> parts_;
	// ---- メンバ関数 ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
	void DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	tnl::Quaternion InverseKinematics(float delta_time);
	void DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back);

};