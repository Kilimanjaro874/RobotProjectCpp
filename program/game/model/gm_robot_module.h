#pragma once
#include "gm_model.h"

class MdlRobotModule : public Model {
	// ----- 1リンク分のロボットモジュール作成(モデルを継承) ------ //
public:
	// ---- ひとまず１パーツのみで構成 ---- //
	enum {
		e_link01,		// リンクパーツ
		e_axis01,		// 回転軸パーツ
		e_axis02,		// 回転軸パーツ
		e_axis03,		// 回転軸パーツ
		e_parts_max
	};



	// ---- メンバ変数定義 ---- //
	// --- DKパラメータ追加 ---
	// pos_ : モジュール原点の位置ベクトルとして使用(oΣi基準)
	// rot_ : モジュールの局所的な回転として使用(iΣi基準)
	tnl::Vector3 rotAi_;		// 初期モジュール回転軸(oΣi基準)
	tnl::Vector3 posLi_;		// 初期モジュールから次のモジュール原点へのベクトル(oΣi基準)
	tnl::Vector3 oLi_;			// Σo基準の平行移動量格納

	// --- エージェントからモデルを参照できるよう、以下定義 --- //
	
	void update(float delta_time) override;
	static MdlRobotModule* Create();
	void localDK(tnl::Quaternion q_back, tnl::Vector3 l_back);

	void localDK(tnl::Quaternion q_back, tnl::Vector3 l_back);			// 本モジュールでの順運動計算実施

};