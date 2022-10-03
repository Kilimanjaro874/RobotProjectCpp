#pragma once
#include "gm_parts.h"

class FaceVec {
	// ----- 有顔ベクトルを定義 ----- //
	// 有顔ベクトル：ロボット工学一部界隈で使う用語。ある点pos_に2軸の直行した単位ベクトルを定義する事で、
	// ゲームオブジェクトの位置・姿勢を本クラスで表現する事ができる。(dir_y_が欲しければdir_x_, dir_z_を外積すれば良い)
	// Module IK計算の制御目標位置・姿勢 or 制御対象位置・姿勢の表現のため、本クラスを使用する。
	// 可視化したければ、Partsクラスを本クラスに登録すれば良い。
public:
	FaceVec() {};
	virtual ~FaceVec() {
		for (auto pts : parts_) delete pts;
	}
	// ---- メンバ変数 ---- //
	// --- for DK, IK --- 
	tnl::Vector3 pos_;		// 位置（どの座標系Σに属するかはユーザーの設定次第：位置参照用)
	tnl::Vector3 dir_z_;	// 方向単位ベクトルz(人間でいう、人差し指を目標へ向けた時のその方向を単位ベクトルとして表現:姿勢参照用)
	tnl::Vector3 dir_x_;	// 方向単位ベクトルx(人差し指に対し、直行した親指の方向を単位ベクトルとして表現：姿勢参照用)
	tnl::Quaternion rot_sum_;	// 本クラス初期姿勢からどれだけ回転したかを格納しておく。
	// --- for render --- 
	std::vector<Parts*> parts_;		// パーツクラス
	enum {
		e_origine_,
		e_dir_z_,
		e_dir_x_,
		e_parts_max
	};
	// ---- メンバ関数 ---- //
	// 必要な機能を都度、追加していく予定
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	static FaceVec* Create(tnl::Vector3 pos_);
	void Rotate(tnl::Quaternion rot_temp);
};