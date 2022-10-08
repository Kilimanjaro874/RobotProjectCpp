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
	rot_sum_ = rot_sum_ * q_back;
	rot_axis_ = tnl::Vector3::TransformCoord(init_rot_axis_, rot_sum_);
	rot_tmp_ = tnl::Quaternion::RotationAxis(rot_axis_, 0);
	rot_sum_ = rot_sum_ * rot_tmp_;
	dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	dir_x_ = tnl::Vector3::TransformCoord(init_dir_x_, rot_sum_);
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// 次モジュールの位置を格納
	
}

void Module::DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- 順運動学：ワールド座標系Σo基準に計算 ---- //
	// 逆運動学計算有りVer.
	// --- 位置・回転軸更新 ---
	pos_o_ = p_back;
	rot_sum_ *= q_back;
	rot_axis_ = tnl::Vector3::TransformCoord(init_rot_axis_, rot_sum_);
	rot_axis_.normalize();
	tnl::Quaternion iqi = InverseKinematics(delta_time);
	rot_tmp_ = q_back * iqi;
	rot_sum_ *= iqi;
	dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	dir_z_.normalize();			// 丸め誤差対策：正規化
	dir_x_ = tnl::Vector3::TransformCoord(init_dir_x_, rot_sum_);
	dir_x_.normalize();			// 丸め誤差対策：正規化
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// 次モジュールの位置を格納
}

tnl::Quaternion Module::InverseKinematics(float delta_time) {
	// ---- 逆運動学計算：ワールド座標系Σo基準
	// ik_settingの値に応じてtarget, objectで解くIKの種類が変化する
	// 回転軸周りの微小角度dthはフレームレート変化で挙動が損なわれないよう、刻み時間を乗じておく(標準60fps想定)
	// <パラメータ>
	
	// --- 微小角度計算 ---
	float dth = 0;
	float dth_sum = 0;
	tnl::Vector3 tmp_pe;
	tnl::Vector3 tmp_pr;
	tnl::Vector3 x;
	tnl::Vector3 y;
	for (int i = 0; i < ik_settings_.size(); i++) {
		switch (ik_settings_[i].ik_type)
		{
		case(pos_to_pos):
			tmp_pe = ik_settings_[i].object->pos_ - pos_o_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirz_to_pos):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirx_to_pos):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirz_to_dirz):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->dir_z_;
			break;
		case(dirx_to_dirx):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->dir_x_;
			break;
		case(dirz_to_dirx):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->dir_x_;
			break;
		case(dirx_to_dirz):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->dir_z_;
			break;

		default:
			printf("error: ik_typeの設定を見直してください");
			break;
		}
		// --- IK処理開始 --- //
		tnl::Vector3 x = tnl::Vector3::Cross(tmp_pe, rot_axis_);
		tnl::Vector3 y = tnl::Vector3::Cross(tmp_pr, rot_axis_);
		dth = delta_time * 60.0 * ik_settings_[i].kp * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// 回転軸上にtarget or object : 特異点->dth = 0でエラー回避
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// 回転方向決定
		dth *= rot_axis_.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	
	// 回転量をクォータニオンで返す
	return tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
}

