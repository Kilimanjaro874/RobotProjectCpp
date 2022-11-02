#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	Robot() {};
	~Robot() {
		for (int n = 0; n < modules_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
		for (int n = 0; n < targets_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
		for (int n = 0; n < objects_.size(); n++) {
			for (auto mod : modules_[n]) delete mod;
		}
	}
	// ----- ���{�b�g�̍\�� ----- //
	enum {
		e_lower_back_,
		e_body_,
		e_r_leg,
		e_l_leg,
		e_head,
		e_r_arm,
		e_l_arm,
		e_modules_max
	};
	// ----- ���[���h���W���猩���{���{�b�g��DK�p�����[�^ ----- //
	std::vector<dk_setting> rob_dk_s_v_;
	// ----- Render & DK & IK ------ //
	std::vector<std::vector<Module*>> modules_;
	std::vector<std::vector<Module*>> targets_;		// IK�̃^�[�Q�b�g��o�^
	std::vector<std::vector<Module*>> objects_;		// IK�̑���Ώۂ�o�^
	// ----- ���{�b�g�ړ��p�����[�^ ----- //
	float move_speed_ = 50.0;		// �ړ����x : *= delta_time
	float rotate_speed_ = 1.5f;		// ���񑬓x : *= delta_time
	tnl::Vector3 d_move_;			// 1�t���[���Ԃ�x, z�ړ��ʂ��i�[
	tnl::Quaternion rot_move_;		// 1�t���[���Ԃ̉�]�ʂ��i�[
	// ----- ���{�b�g���W���_���烋�[�g���W�ւ̈ʒu�x�N�g�� ----- //

	// ----- �����o�֐� ------ //
	static Robot* Create(const tnl::Vector3& pos, const tnl::Quaternion& rot);

	// ---- mode01 : TPS�ˌ����[�h ---- //
	void mode01_init(float delta_time);
	void mode01_update(float delta_time);
	// --- �L�����R�� --- //
	void move(float delta_time);
	void moveDK();
	// --- �^�[�Q�b�gDK --- //
	void targetInit();
	void targetDK();
};