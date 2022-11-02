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

void Module::InitParams(int id, tnl::Vector3 rot_axis, tnl::Quaternion rot_sum, 
	tnl::Vector3 dir_z, tnl::Vector3 dir_x){
	// ---- モジュールパラメータ初期化のために使用 ---- //
	id_ = id;
	in_rot_axis_ = rot_axis;
	rot_sum_ = rot_sum;
	in_dir_z_ = dir_z;
	in_dir_x_ = dir_x;
}

void Module::InitDK(const std::vector<dk_setting>& dks) {
	// ---- 順運動学のための初期化処理 ---- //
	bool do_DK = false;
	if (id_ == 502) {
		printf("deb");
	}
	for (auto d : dks) {
		if (d.id_ != NULL && d.id_ == id_) {
			// 本モジュール位置・姿勢・座標系更新
			SelfDK(d.dir_r_n_ * d.dir_r_length_, d.q_r_n_);
			do_DK = true;
		}
	}
	if (!do_DK) { return; }		// DKの対象外であれば関数を抜ける.

	//　次のモジュール位置・姿勢を更新するためのDKパラメータ初期化
	for (int i = 0; i < dk_s_v_.size(); i++) {
		if (id_ == 601) {
			printf("deb");
		}
		// ベクトルを単位ベクトル(方向), 長さに分解。数値計算累計誤差を避ける為
		dk_s_v_[i].dir_r_length_ = dk_s_v_[i].dir_r_n_.length();	// 距離
		if (dk_s_v_[i].dir_r_length_ == 0) {dk_s_v_[i].dir_r_length_ = 1.0; }
		dk_s_v_[i].dir_r_n_.normalize();
		dk_s_v_[i].q_r_n_ *= rot_tmp_;
	}

	// DKパラメータ初期値作成
	in_dk_s_v_.resize(dk_s_v_.size());
	in_dk_s_v_ = dk_s_v_;			// 初期値としてコピー。Update()では、次モジュールの位置・姿勢を本構造体で表現＆更新していく
	// DKパラメータ更新値作成
	for (int i = 0; i < in_dk_s_v_.size(); i++) {
		dk_s_v_[i].dir_r_n_ = pos_o_ + 
			tnl::Vector3::TransformCoord(in_dk_s_v_[i].dir_r_n_, in_dk_s_v_[i].q_r_n_) * in_dk_s_v_[i].dir_r_length_;
		dk_s_v_[i].dir_r_length_ = 1.0;
	}
}

void Module::SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ---- 順運動学のための処理：自身の位置・姿勢を引数情報を元に更新 ---- //
	pos_o_ = pos;
	rot_sum_ *= rot;
	rot_tmp_ = rot;
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
	float dth = 0;
	float dth_sum = 0;
	if (ik_s_v_.size() != 0) {
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
	}
	tnl::Quaternion tmp_rot = tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);

	// --- DK実施 --- //
	rot_tmp_ *= tmp_rot;
	rot_sum_ *= tmp_rot;
	dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);	// z軸
	dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);	// x軸
	// --- DK_setting更新 --- //
	for (int i = 0; i < dk_s_v_.size(); i++) {
		dk_s_v_[i].dir_r_n_ = pos_o_ + 
			tnl::Vector3::TransformCoord(in_dk_s_v_[i].dir_r_n_, rot_sum_) * in_dk_s_v_[i].dir_r_length_;
		dk_s_v_[i].q_r_n_ = rot_tmp_;
	}

}

void Module::AllInitDK(const Module* mod, const std::vector<dk_setting>& dks) {
	// --- 再帰的にModuleの運動学実行。ロボット初期位置・姿勢を完成させる --- //
	// preorderで実行する事
	if (this->id_ == 200) {
		printf("deb");
	}

	InitDK(dks);
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		//AllInitDK(next[i], dk_s_v_);
		next[i]->AllInitDK(next[i], dk_s_v_);
	}
}

void Module::updateTree(const Module* mod, float delta_time) {
	// --- 登録したモジュール群を preorderで更新していく --- //
	update(delta_time);
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->updateTree(next[i], delta_time);
	}

}

void Module::renderTree(const Module* mod, dxe::Camera* camera) {
	// --- 登録したモジュール群を preorderで描画していく --- //
	if (is_render_) { render(camera); }
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->renderTree(next[i], camera);
	}

}

void Module::AttachModule(Module* mod, Module* attach_mod, int id) {
	// --- 既に登録したモジュール群の特定のidに、あるモジュールを登録する：再帰 --- //
	if (id == id_) {
		// 親子設定
		next.push_back(attach_mod);
		attach_mod->back = mod;
		attach_mod->InitDK(mod->dk_s_v_);	//DK処理
		return;
	}
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->AttachModule(next[i], attach_mod, id);
	}
}

void Module::SetIKParams(Module* mod, int id, Module* target, Module* object, 
	int ikType, float kp) {
	// --- あるidのモジュールに、IK目標モジュール、IK制御対象をセットする：再帰 --- 
	if (id == id_) {
		ik_s_v_.push_back({ ikType, kp, target, object });
		return;
	}
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->SetIKParams(next[i], id, target, object, ikType, kp);
	}
}

void Module::AllupdateIK(const Module* mod, float delta_time) {
	// --- モジュールのIKを実施する：再帰 --- //
	if (id_ == 600) {
		printf("deb");
	}
	DKwithIK(delta_time, mod->dk_s_v_);
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->AllupdateIK(this, delta_time);
	}
}