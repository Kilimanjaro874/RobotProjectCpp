#include "gm_module.h"

void Module::update(float delta_time) {
	// ---- 部品位置・姿勢更新 ---- //
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_o_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera* camera) {
	// ---- 部品描画 ---- //
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}


void Module::InitDK(const std::vector<dk_setting>& dks) {
	// ---- 順運動学のための初期化処理 ---- //
	// --- 座標系初期化 --- //
	bool do_DK = false;
	for (auto d : dks) {
		if (d.id_ == id_) {
			pos_o_ = d.dir_r_n_ * d.dir_r_length_;				// 位置
			rot_sum_ *= d.q_r_n_;								// 姿勢
			do_DK = true;
			break;
		}
	}
	if (!do_DK) { return; }		// DKの対象外であれば関数を抜ける.
	in_rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);
	in_rot_axis_.normalize();		// 回転軸
	in_dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	in_dir_z_.normalize();			// モジュールz軸方向単位ベクトル(座標系として使用)
	dir_z_ = in_dir_z_;
	in_dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	in_dir_x_.normalize();			// モジュールx軸方向単位ベクトル(座標系として使用)
	dir_x_ = in_dir_x_;
	// --- DKセッティング初期化 --- //
	for (auto dks : dk_s_v_) {
		dks.dir_r_length_ = dks.dir_r_n_.length();	// 次モジュールまでの距離格納
		dks.dir_r_n_.normalize();
		dks.dir_r_n_ = tnl::Vector3::TransformCoord(dks.dir_r_n_, rot_sum_);
		dks.dir_r_n_.normalize();					// 次モジュールまでの方向単位ベクトル格納
	}
}

void Module::SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ---- 順運動学のための処理：自身の位置・姿勢を引数情報を元に更新 ---- //
	pos_o_ = pos;
	rot_sum_ *= rot;
	in_rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);
	in_rot_axis_.normalize();		// 回転軸
	in_dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	in_dir_z_.normalize();			// モジュールz軸方向単位ベクトル(座標系として使用)
	dir_z_ = in_dir_z_;
	in_dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	in_dir_x_.normalize();			// モジュールx軸方向単位ベクトル(座標系として使用)
	dir_x_ = in_dir_x_;
}

void Module::DKwithIK(float delta_time, const std::vector<dk_setting>& dks) {
	// ---- 順運動学＆逆運動学計算 ---- //
	// --- 座標系初期化 --- //
	bool do_DK = false;
	for (auto d : dks) {
		if (d.id_ == id_) {
			pos_o_ = d.dir_r_n_ * d.dir_r_length_;				// 位置
			rot_sum_ *= d.q_r_n_;
			rot_tmp_ = d.q_r_n_;
			do_DK = true;
			break;
		}
	}
	if (!do_DK) { return; }		// DKの対象外であれば関数を抜ける.
	rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);	// 回転軸
	// --- IK実施 --- //
	if (ik_s_v_.size() == 0) { return; }	// IKを実施しないならば、関数を抜ける.
	float dth = 0;
	float dth_sum = 0;
	tnl::Vector3 tmp_pe;	// 制御対象位置 or 姿勢ベクトル
	tnl::Vector3 tmp_pr;	// 制御目標位置 or 姿勢ベクトル
	tnl::Vector3 x;			// 計算結果一時格納用
	tnl::Vector3 y;			// 計算結果一時格納用
	for (int i = 0; i < ik_s_v_.size(); i++) {
		switch (ik_s_v_[i].ik_type_)
		{
		case(pos_to_pos):
			tmp_pe = ik_s_v_[i].object_->pos_o_ - pos_o_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirz_to_pos):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirx_to_pos):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirz_to_dirz):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->dir_z_;
			break;
		case(dirx_to_dirx):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->dir_x_;
			break;
		case(dirz_to_dirx):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->dir_x_;
			break;
		case(dirx_to_dirz):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->dir_z_;
			break;

		default:
			printf("error: ik_typeの設定を見直してください");
			break;
		}
		tnl::Vector3 x = tnl::Vector3::Cross(tmp_pe, rot_axis_);
		tnl::Vector3 y = tnl::Vector3::Cross(tmp_pr, rot_axis_);
		dth = delta_time * 60.0 * ik_s_v_[i].kp_ * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// 回転軸上にtarget or object : 特異点->dth = 0でエラー回避
		if (dth > tnl::PI / 24) {
			dth = tnl::PI / 24;
		}
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// 回転方向決定
		dth *= rot_axis_.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	tnl::Quaternion tmp_rot = tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
	// --- DK実施 --- //
	rot_tmp_ *= tmp_rot;
	rot_sum_ *= tmp_rot;
	dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);	// z軸
	dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);	// x軸
	// --- DK_setting更新 --- //
	for (auto dks : dk_s_v_) {
		dks.q_r_n_ = rot_tmp_;
		dks.dir_r_n_ = tnl::Vector3::TransformCoord(dks.dir_r_n_, rot_tmp_);
		dks.dir_r_n_.normalize();
	}
}