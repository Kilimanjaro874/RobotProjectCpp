#pragma once
#include "gm_parts.h"
#include "gm_module.h"

class FaceVector : public Module{
	// ----- 有顔ベクトルを定義 ----- //
	// 有顔ベクトル：ロボット工学一部界隈で使う用語。ある点pos_に2軸の直行した単位ベクトルを定義する事で、
	// ゲームオブジェクトの位置・姿勢を本クラスで表現する事ができる。(dir_y_が欲しければdir_x_, dir_z_を外積すれば良い)
	// Module IK計算の制御目標位置・姿勢 or 制御対象位置・姿勢の表現のため、本クラスを使用する。
public:
	// ----- Render ----- //
	enum {
		e_origine,
		e_dir_z,
		e_dir_x,
		e_parts_max
	};
	float sphere_size_ = 2.0;
	float bar_length_ = 20.0;
	// ----- メンバ関数 ----- //
	static FaceVector* Create(int id, tnl::Vector3 pos, tnl::Quaternion rot);
};