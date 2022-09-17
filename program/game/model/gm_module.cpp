#include "gm_module.h"

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

void Module::localDirectKinematics(const tnl::Vector3& p_back, const tnl::Quaternion& q_back, const tnl::Vector3& l_back) {
	// ----- 局所的順運動学実施 ----- //	
	// 本モジュールを[i]とし、[i-1]モジュールからの回転量q_back, 平行移動量l_backを基に、
	// ある座標系を基準とした(どの座標系を基準とするかは本クラス運用による)位置・姿勢の
	// 更新を実施後、自身の回転の影響を[i+i]モジュールへ回転量q_i, 平行移動量l_iとして渡す
	// --- 位置・回転軸ベクトル更新 ---
	pos_ = p_back;
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	
	// 正規化
	rotAi_.normalize();
	

	// --- 逆運動学処理 ---
	// test
	float dth = tnl::ToRadian(2.0);
	// --- 本モジュールの局所的な回転iΣiから回転量、平行移動量生成 ---
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, dth);




	//tnl::Vector3 ili = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
	// --- 本モジュールの局所的な回転iΣiをある基準座標x基準へ変換&格納：xΣi
	rot_ = iqi * q_back;

	link_dir = tnl::Vector3::TransformCoord(link_dir, rot_);
	link_dir.normalize();


	rot_sum_ *= rot_;		// 初期姿勢からの回転量積算値を格納：パーツの位置・姿勢更新のため
	// --- 平行移動量も同様、局所的な回転による平行移動の影響をxΣi基準で表す＆格納
	//xli_ = tnl::Vector3::TransformCoord(l_back, iqi) + ili;
	
	pos_next = pos_ + link_dir * link_length;

}
//
//void Module::localDirectKinematics(const tnl::Quaternion& q_back, const tnl::Vector3& l_back) {
//	// ----- 局所的順運動学実施 ----- //	
//	// 本モジュールを[i]とし、[i-1]モジュールからの回転量q_back, 平行移動量l_backを基に、
//	// ある座標系を基準とした(どの座標系を基準とするかは本クラス運用による)位置・姿勢の
//	// 更新を実施後、自身の回転の影響を[i+i]モジュールへ回転量q_i, 平行移動量l_iとして渡す
//	// --- 位置・回転軸ベクトル更新 ---
//	pos_ = tnl::Vector3::TransformCoord(pos_, q_back) + l_back;
//	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
//	// --- 逆運動学処理 ---
//	// test
//	float dth = tnl::ToRadian(10.0);
//	// --- 本モジュールの局所的な回転iΣiから回転量、平行移動量生成 ---
//	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, dth);
//	tnl::Vector3 ili = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
//	// --- 本モジュールの局所的な回転iΣiをある基準座標x基準へ変換&格納：xΣi
//	rot_ = iqi * q_back;
//	rot_sum_ *= rot_;		// 初期姿勢からの回転量積算値を格納：パーツの位置・姿勢更新のため
//	// --- 平行移動量も同様、局所的な回転による平行移動の影響をxΣi基準で表す＆格納
//	xli_ = tnl::Vector3::TransformCoord(l_back, iqi) + ili;
//
//}
//
