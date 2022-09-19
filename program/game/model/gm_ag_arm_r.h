#pragma once
#include "gm_module.h"

class MdlArm_r : public Module {
	// ----- ���{�b�g�F�E�r��z��:���W���[�����܂Ƃ߂�����G�[�W�F���g�Ƃ��ē��� ----- //
public:
	enum {
		e_shoulder,		// ��
		e_arm,			// �r
		e_wrist,		// ���
		e_models_max	// �����
	};
	std::vector<Module*>modules_;		// ���W���[���̃x�N�^�[�^
	std::vector<tnl::Vector3> pos_rs_;	// �e���W���[���̎�搧��ڕW�ʒu���i�[
	std::vector<tnl::Vector3> pos_es_;	// �e���W���[���̎�搧��ڕW�ʒu�֒Ǐ]�����������ʒu�����i�[
	tnl::Vector3 forward_;
	tnl::Quaternion tempQ_;
	tnl::Vector3 target_pos_;
	tnl::Vector3 target_es;

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	// ---- �e���W���[����LocalDirectKinematics���G�[�W�F���g�Ƃ��Ď��{ ---- //
	void calcLDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	void calcLDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
		const tnl::Vector3& pos_e, const tnl::Vector3& pos_r);
	// --- �e���W���[���̖ڕW�ʒu�E���䂷����ʒu�̃��X�g�쐬 --- //
	



	void testmove();

	static MdlArm_r* Create(tnl::Vector3& target_pos);
};