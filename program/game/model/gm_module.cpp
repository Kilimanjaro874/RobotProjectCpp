#include "gm_module.h"

void Module::update(float delta_time) {
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_o_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera* camera) {
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- 順運動学 : ワールド座標系Σo基準に計算---- //
	// 逆運動学計算無しVer.
	// --- 位置・回転軸更新 ---
	pos_o_ = p_back;
	rot_axis_ = tnl::Vector3::TransformCoord(rot_axis_, q_back);
	rot_axis_.normalize();		// 丸め誤差対策：正規化
	// --- IK無し：無回転のクォータニオン生成
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rot_axis_, 0);
	rot_tmp_ = iqi * q_back;		// 本モジュールの局所的な回転を生成
	rot_sum_ *= rot_tmp_;		// 初期姿勢からどれだけ回転したかを格納
	// --- 方向ベクトル更新 ---
	dir_z_ = tnl::Vector3::TransformCoord(dir_z_, rot_tmp_);
	dir_z_.normalize();			// 丸め誤差対策：正規化
	dir_x_ = tnl::Vector3::TransformCoord(dir_x_, rot_tmp_);
	dir_x_.normalize();			// 丸め誤差対策：正規化
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// 次モジュールの位置を格納 
}

void Module::DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- 順運動学：ワールド座標系Σo基準に計算 ---- //
	// 逆運動学計算有りVer.
	// --- 位置・回転軸更新 ---
	pos_o_ = p_back;
	rot_axis_ = tnl::Vector3::TransformCoord(rot_axis_, q_back);
	rot_axis_.normalize();		// 丸め誤差対策：正規化
	// --- IK有り：位置 or 姿勢のクォータニオン生成
	tnl::Quaternion iqi = InverseKinematics(delta_time);
	rot_tmp_ = iqi * q_back;
	rot_sum_ *= rot_tmp_;
	// --- 方向ベクトル更新 --- 
	dir_z_ = tnl::Vector3::TransformCoord(dir_z_, rot_tmp_);
	dir_z_.normalize();			// 丸め誤差対策：正規化
	dir_x_ = tnl::Vector3::TransformCoord(dir_x_, rot_tmp_);
	dir_x_.normalize();			// 丸め誤差対策：正規化
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// 次モジュールの位置を格納 
}

tnl::Quaternion Module::InverseKinematics(float delta_time) {
	// ---- 逆運動学計算：ワールド座標系Σo基準
	// cnt_objectsをcnt_targetの状態へ本モジュール回転軸周りで近づくようIKを計算
	// 回転軸周りの微小角度dthはフレームレート変化で挙動が損なわれないよう、刻み時間を乗じておく(標準60fps想定)
	// <パラメータ>
	// pos : 位置へ近づけるIK計算
	// rotz : 姿勢z 方向へ近づけるIK計算
	// rotx : 姿勢x 方向へ近づけるIK計算
	// --- 微小角度計算 ---
	float dth = 0;
	float dth_sum = 0;
	if (is_posIK) {
		for (int i = 0; i < kp_p_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->pos_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->pos_), rot_axis_);
			dth = delta_time * 60.0 * kp_p_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// 回転軸上にcnt_target : 特異点：dth = 0
			// 回転方向決定
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}

	if (is_dir_zIK) {
		for (int i = 0; i < kp_rz_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->dir_z_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->dir_z_), rot_axis_);
			dth = delta_time * 60.0 * kp_rz_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// 回転軸上にcnt_target : 特異点：dth = 0
			// 回転方向決定
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}

	if (is_dir_xIK) {
		for (int i = 0; i < kp_rx_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->dir_x_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->dir_x_), rot_axis_);
			dth = delta_time * 60.0 * kp_rx_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// 回転軸上にcnt_target : 特異点：dth = 0
			// 回転方向決定
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}
	// 回転量を返す
	return tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
}

void Module::UpdateCnt_obj(FaceVec& cnt_object) {
	cnt_object.pos_ = pos_o_;
	cnt_object.dir_z_ = dir_z_;
	cnt_object.dir_x_ = dir_x_;
	cnt_object.rot_sum_ = rot_sum_;
	cnt_object.update(0);
}