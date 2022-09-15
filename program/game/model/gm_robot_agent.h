#pragma once 
#include "gm_model.h"
#include "gm_robot_module.h"

class MdlRobotAgent_Rarms : public MdlRobotModule {
	// ----- MdlRobotModule���G�[�W�F���g�Ƃ��ĊǗ�����N���X�쐬 ----- //
public:
	// ---- �ЂƂ܂����{�b�g�̉E�r�Ƃ��ō\�� ---- //
	enum {
		e_shoulder,		// ��
		e_arm,			// �r
		e_wrist,			// ���
		e_models_max
	};

	tnl::Vector3 pos_;		// �G�[�W�F���g���_�̈ʒu�x�N�g���Ƃ��Ďg�p(p��i�)
	tnl::Quaternion rot_ ;	// �G�[�W�F���g�̋Ǐ��I�ȉ�]�Ƃ��Ďg�p(p��i�)
	tnl::Vector3 rotAi_;
	tnl::Vector3 posLi_;
	tnl::Vector3 oLi = { 0, 0, 0 };

	std::vector<MdlRobotModule*>modules_;	// ���f���̃x�N�g���^

	void update(float delta_time);
	void render(dxe::Camera* camera);
	static MdlRobotAgent_Rarms* Create();
	void directKinematics();
};