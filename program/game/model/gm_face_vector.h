#pragma once
#include "gm_parts.h"

class FaceVector {
	// ----- 有顔ベクトルを定義 ----- //
	// 有顔ベクトル：ロボット工学一部界隈で使う用語。ある点pos_に2軸の直行した単位ベクトルを定義する事で、
	// ゲームオブジェクトの位置・姿勢を本クラスで表現する事ができる。(dir_y_が欲しければdir_x_, dir_z_を外積すれば良い)
	// Module IK計算の制御目標位置・姿勢 or 制御対象位置・姿勢の表現のため、本クラスを使用する。
public:
	FaceVector() {};
	virtual ~FaceVector() {

	}
	std::vector<Parts*> parts_;		// パーツクラス
	enum {
		e_origine_,
		
	};
};