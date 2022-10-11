#pragma once
#include "gm_agent.h"

class Robot : public Agent {
	// ----- ���{�b�g�N���X�F�r���A�������̃G�[�W�F���g�𓝊����ĊǗ�����N���X ----- //

public:
	Robot() {};
	~Robot() {
		for (auto agn : agents_) delete agn;
	}
	enum {
		e_arm_r,
		e_lower_back,
		e_agents_max		// �����
	};
	std::vector<Agent*> agents_;	// �G�[�W�F���g��o�^
	// ---- Direct Kinematics : DK�ɕK�v�ȕϐ� ---- //
	tnl::Vector3 dir_r_;		// ���[�g���W�֌����x�N�g����`
	tnl::Vector3 init_dir_r_;	// ���[�g���W�֌����x�N�g����`�i�����l)�@�F�傫����link_length�Œ�`
	float length_to_root;		// ���[�g���W�ւ̃x�N�g�������i�[
	tnl::Quaternion q_dir_r_;	// ���[�g���W�ւ̃N�H�[�^�j�I��
	// DK test
	tnl::Vector3 dir_arm_r_;		// �e�X�g�F�E�r�܂ł̃x�N�g��
	tnl::Vector3 init_dir_arm_r_;	// �e�X�g�F�E�r�܂ł̃x�N�g������
	tnl::Quaternion q_arm_r_;	// 
	float length_to_arm_r_;		// arm_r�ւ̋���


	// ---- �ړ��n�ϐ� ---- //
	float move_speed_ = 50.0f;		// �ړ����x(m/s) (*= delta_time)
	float rotate_speed_ = 1.5f;	// ���񑬓x(rad/s) (*= delta_time)
	tnl::Vector3 d_move_;			// 1�t���[���Ԃ�x,z�ړ��ʂ��i�[


	static Robot* Create(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	void update(float delta_time);
	void render(dxe::Camera* camera);

	void move(float delta_time);
	void mode01_init(const std::vector<FaceVec*> targets) override;	// mode01 : �^�[�Q�b�g�Ɍ������ăG�C������
	void mode01_update(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back, std::vector<FaceVec*> targets) override;
	void mode02_init(FaceVec& target) override;
	void mode02_update()override;
};