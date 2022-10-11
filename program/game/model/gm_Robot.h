#pragma once
#include "gm_agent.h"

class Robot : public Agent {
	// ----- ロボットクラス：腕部、頭部等のエージェントを統括して管理するクラス ----- //

public:
	Robot() {};
	~Robot() {
		for (auto agn : agents_) delete agn;
	}
	enum {
		e_arm_r,
		e_lower_back,
		e_agents_max		// おわり
	};
	std::vector<Agent*> agents_;	// エージェントを登録
	// ---- Direct Kinematics : DKに必要な変数 ---- //
	tnl::Vector3 dir_r_;		// ルート座標へ向くベクトル定義
	tnl::Vector3 init_dir_r_;	// ルート座標へ向くベクトル定義（初期値)　：大きさはlink_lengthで定義
	float length_to_root;		// ルート座標へのベクトル長さ格納
	tnl::Quaternion q_dir_r_;	// ルート座標へのクォータニオン
	// DK test
	tnl::Vector3 dir_arm_r_;		// テスト：右腕までのベクトル
	tnl::Vector3 init_dir_arm_r_;	// テスト：右腕までのベクトル方向
	tnl::Quaternion q_arm_r_;	// 
	float length_to_arm_r_;		// arm_rへの距離


	// ---- 移動系変数 ---- //
	float move_speed_ = 50.0f;		// 移動速度(m/s) (*= delta_time)
	float rotate_speed_ = 1.5f;	// 旋回速度(rad/s) (*= delta_time)
	tnl::Vector3 d_move_;			// 1フレーム間のx,z移動量を格納


	static Robot* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	void update(float delta_time);
	void render(dxe::Camera* camera);

	void move(float delta_time);
	void mode01_init(const std::vector<FaceVec*> targets) override;	// mode01 : ターゲットに向かってエイム動作
	void mode01_update(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back, std::vector<FaceVec*> targets) override;
	void mode02_init(FaceVec& target) override;
	void mode02_update()override;
};