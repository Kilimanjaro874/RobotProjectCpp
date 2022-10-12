#pragma once
#include "gm_module.h"

class MdlRbArmR : public Module {
public:

	MdlRbArmR() {};
	~MdlRbArmR() {
		for (auto mod : modules_) delete mod;
	}

	enum {
		e_sho_x,		// ���Fx�����W���[��
		e_sho_z,		// ���Fz��
		e_arm_x,		// �r
		e_arm_y,		
		e_wrist_x,		// ���
		e_wrist_z,
		e_modules_max	// �����
	};

	std::vector<Module*>modules_;		// ���W���[���Q�ƌ^
	std::vector<tnl::Vector3> pos_rs_;	// ���ڕW�ʒu
	std::vector<tnl::Vector3> pos_es_;	// ����Ώۂ̎��ʒu�i�[

	void update(float delta_time) override;
	void render(dxe::Camera* camera) override;
	static MdlRbArmR* Create(tnl::Vector3 p_back, tnl::Quaternion q_back);
	// ----- ����F�^�[�Q�b�g�Ɍ������Ęr�ŃG�C������֐� ----- //
	void aimTarget(float delta_time, tnl:: Vector3 p_back, tnl::Quaternion q_back, tnl::Vector3 target, tnl::Vector3 elbowRef);
	
};