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
	// ---- Direct Kinematics : DKに必要な変数 ---- //
	tnl::Vector3 pos_a_;			// エージェント座標系xΣaiからの本モジュール座標系原点位置
	tnl::Vector3 pos_o_;			// ワールド座標oΣからの本モジュールiの座標系原点位置
	tnl::Vector3 pos_a_next_;		// エージェント座標系：次モジュールの位置を格納
	tnl::Vector3 pos_o_next_;		// ワールド座標系：次モジュールの位置を格納
	tnl::Vector3 dir_z_;			// 本モジュールiからモジュールi+iへの方向単位ベクトルを定義(位置IK計算に使用)
	tnl::Vector3 init_dir_z_;		// 本モジュールiからモジュールi+iへの方向単位ベクトルを定義・初期状態(位置IK計算に使用)
	tnl::Vector3 dir_x_;			// 本モジュールi姿勢制御のための姿勢方向単位ベクトルを定義(姿勢IK計算に使用)
	tnl::Vector3 init_dir_x_;
	tnl::Vector3 rot_axis_;			// モジュールiの回転軸単位ベクトル：本モジュール原点に立てる事とする
	tnl::Vector3 init_rot_axis_;
	float link_length_;				// dir_z_の大きさを格納：モジュールi原点からモジュールi+iまでの長さを定義
	tnl::Quaternion rot_tmp_;		// IK計算時、oΣ基準に記述された、1フレーム間で本モジュールが取る一時的な回転量を格納
	tnl::Quaternion rot_sum_;		// 本モジュールが初期姿勢からどれだけ回転したかを格納しておく。
	std::vector<float> kp_p_nums_;	// IK：微小角度計算の際に使用する比例定数 ※目標位置参照用：複数個ある場合にも対応のためvector表現 
	std::vector<float> kp_rz_nums_;	// IK：微小角度計算の際に使用する比例定数 ※目標姿勢z参照用：複数個ある場合にも対応のためvector表現
	std::vector<float> kp_rx_nums_;	// IK：微小角度計算の際に使用する比例定数 ※目標姿勢x参照用：複数個ある場合にも対応のためvector表現
	std::vector<FaceVec*> cnt_objects_;		// 有顔ベクトルクラス：制御対象をモジュールに指定
	std::vector<FaceVec*> cnt_targets_;		// 有顔ベクトルクラス：制御目標をモジュールに指定
	bool is_posIK = false;			// IK : 位置参照で解くか否か
	bool is_dir_zIK = false;		// IK : 姿勢z参照で解くか否か
	bool is_dir_xIK = false;		// IK : 姿勢x参照で解くか否か
	// ---- Render ----- //
	std::vector<Parts*> parts_;
	// ---- メンバ関数 ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
	void DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	tnl::Quaternion InverseKinematics(float delta_time);
	void DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back);
	//void SetCnt_obj(FaceVec& cnt_object);
	void Clear_kp_IKstate();
	void UpdateCnt_obj(FaceVec& cnt_object);

};