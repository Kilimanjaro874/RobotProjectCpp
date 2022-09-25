#pragma once
#include "gm_parts.h"

class Module {
public:
	Module() {};


	// ---- Direct Kinematics : DKに必要な変数 ---- //
	tnl::Vector3 pos_a_;		// エージェント座標系xΣaiからの本モジュール座標系原点位置
	tnl::Vector3 pos_o_;		// ワールド座標oΣからの本モジュールiの座標系原点位置
	tnl::Vector3 dir_z_;		// 本モジュールiからモジュールi+iへの方向単位ベクトルを定義(位置IK計算に使用)
	tnl::Vector3 dir_x_;		// 本モジュールi姿勢制御のための姿勢方向単位ベクトルを定義(姿勢IK計算に使用)
	tnl::Vector3 rot_axis;		// モジュールiの回転軸単位ベクトル：本モジュール原点に立てる事とする
	float link_length_;			// dir_z_の大きさを格納：モジュールi原点からモジュールi+iまでの長さを定義
	tnl::Quaternion rot_tmp;	// IK計算時、oΣ基準に記述された、1フレーム間で本モジュールが取る一時的な回転量を格納
	tnl::Quaternion rot_sum;	// 本モジュールが初期姿勢からどれだけ回転したかを格納しておく。
	std::vector<float> kp_num;	// IK：微小角度計算の際に使用する比例定数 ※目標位置・姿勢が複数個ある場合にも対応のためvector表現
	
	

};