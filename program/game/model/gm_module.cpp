#include "gm_module.h"
//#include <cmath>

void Module::update(float delta_time) {
	// ----- LDK実施後、本モデルに付属のパーツ群の位置・姿勢自動更新 ----- //
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera * camera) {
	// ----- 本モデルに付属のパーツ群レンダー(Modelのまま) ----- //
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::localDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- 局所順運動学実施 ----- //	
	// 本モジュールを[i]とし、[i-1]モジュールからの回転量q_back, 位置p_backを基に、
	// ある座標系oΣxを基準とした(どの座標系を基準とするかは本クラス運用による)位置・姿勢の
	// 更新を実施後、自身の回転の影響を[i+i]モジュールへ回転量rot_, 位置pos_nextに格納する
	// 逆運動学計算も内部で実施
	// --- 位置・回転軸ベクトル更新 ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// 正規化
	rotAi_.normalize();
	// --- LIK実施後、本モジュールの局所的な回転iΣiから回転量、次モジュールの位置計算 ---
	tnl::Quaternion iqi = localInverseKinematics(delta_time);
	// --- 本モジュールの局所的な回転iΣiをある基準座標x基準へ変換&格納：xΣi
	rot_ = iqi * q_back;
	rot_sum_ *= rot_;		// 初期姿勢からの回転量積算値を格納：パーツの位置・姿勢更新のため
	// --- 本モジュールの局所的な回転iΣiをlinkに作用＆xΣi基準の座標に変換＆次モジュールの位置をxΣi基準で格納する。
	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();
	pos_next = pos_ + link_dir * link_length;
}

void Module::localDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ----- 局所順運動学実施 ----- //	
	// 本モジュールを[i]とし、[i-1]モジュールからの回転量q_back, 位置p_backを基に、
	// ある座標系oΣxを基準とした(どの座標系を基準とするかは本クラス運用による)位置・姿勢の
	// 更新を実施後、自身の回転の影響を[i+i]モジュールへ回転量rot_, 位置pos_nextに格納する
	// 逆運動学計算は実施しない
	// --- 位置・回転軸ベクトル更新 ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// 正規化
	rotAi_.normalize();
	// --- LIK無し ---
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, 0);
	// --- 本モジュールの局所的な回転iΣiをある基準座標x基準へ変換&格納：xΣi
	rot_ = iqi * q_back;
	rot_sum_ *= rot_;		// 初期姿勢からの回転量積算値を格納：パーツの位置・姿勢更新のため
	// --- 本モジュールの局所的な回転iΣiをlinkに作用＆xΣi基準の座標に変換＆次モジュールの位置をxΣi基準で格納する。
	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();
	pos_next = pos_ + link_dir * link_length;
}

tnl::Quaternion Module::localInverseKinematics(float delta_time) {
	// ----- 局所逆運動学実施 ----- // 
	// 本モジュールの回転軸に垂直な平面上で、手先位置pos_e, 目標位置pos_rを近づける回転量(クォータニオン）を生成、
	// ただし、回転量は次モジュールへ次々と大きく伝播していくため、回転量を微小回転とするため比例定数kpを乗ずる事で、
	// 手先位置が目標位置へ漸近していくような挙動の実現を目指す。
	// また、回転軸周りの微小角度dthはフレームレート変化で挙動が損なわれないよう、刻み時間を乗じておく(標準60fps想定)。
	// 外積結果：右手系
	// ※ 計算前にpos_r_, pos_e_を更新しておく事
	// ---- 微小角度計算 -----
	float dth = 0;
	float dth_sum = 0;
	for (int i = 0; i < kp_num_.size(); i++) {
		tnl::Vector3 x = tnl::Vector3::Cross((pos_e_[i] - pos_), rotAi_);
		tnl::Vector3 y = tnl::Vector3::Cross((pos_r_[i] - pos_), rotAi_);
		// 回転軸延長上に手先目標位置、制御手先位置がある場合は微小角度を0とする
		dth = delta_time * 60.0 * kp_num_[i] * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length()
			, (float)-1, float(1)));
		if (!isfinite(dth)) { dth = 0; }
		// ---- 回転方向の決定 ---- 
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
		float sign = rotAi_.dot(axis) > 0 ? 1 : -1;
		dth *= sign;		// 方向決定
		dth_sum += dth;		// n回の逆運動学の結果を返す
	}

	// ---- 局所的(iΣi)な回転量：クォータニオンを返す ---- //
	return tnl::Quaternion::RotationAxis(rotAi_, dth_sum);

}