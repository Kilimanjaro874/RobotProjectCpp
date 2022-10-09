#pragma once
#include "gm_module.h"

class Agent : public Module {
public:
	Agent() {};
	~Agent() {
		for (auto mod : modules_) delete mod;
	}
	std::vector<Module*> modules_;		// ���W���[����o�^
	std::vector<FaceVec*> targets_;		// IK�̖ڕW�ʒu�E�p�����X�g
	std::vector<FaceVec*> cnt_objects_;	// IK�̐���Ώۂ̈ʒu�E�p�����X�g

	// ---- �G�[�W�F���g�ŋ��ʂ̍�Ƃ����X�ǉ��\�� ---- //
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	virtual void mode01_init(const std::vector<FaceVec*> targets) = 0;		// �G�[�W�F���g�ɗ^���郂�[�V�����P�F������
	virtual void mode01_update(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back, std::vector<FaceVec*> targets);	// �V update
	virtual void mode02_init(FaceVec& target);
	virtual void mode02_update();

};