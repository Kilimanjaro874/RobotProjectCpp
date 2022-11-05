#pragma once
#include "gm_module.h"

class Robot : public Module{
public:
	//// ------ �����o�ϐ� ------ ////
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
	
	// ----- ���{�b�g�ړ��p�����[�^ ----- //
	float move_speed_ = 50.0;		// �ړ����x : *= delta_time
	float rotate_speed_ = 1.5f;		// ���񑬓x : *= delta_time
	tnl::Vector3 d_move_;			// 1�t���[���Ԃ�x, z�ړ��ʂ��i�[
	tnl::Quaternion rot_move_;		// 1�t���[���Ԃ̉�]�ʂ��i�[
	std::vector<dk_st> _dk_input;	// �R���g���[���[�����DK�����i�[
	// ----- csv���烂�W���[�����\�z���邽�߂̃p�����[�^ ----- //
	
	


	//// ------ �����o�֐� ------- ////
	static Robot* Create(const tnl::Vector3 pos, const tnl::Quaternion rot);
	void init(Robot* rob, int id, std::string name, 
		tnl::Vector3 pos, tnl::Vector3 rot_axis, tnl::Quaternion rot);
	// ---- mode01 : TPS�ˌ����[�h ---- //


	// ---- CSV�f�[�^�ǂݍ��݊֐� ---- //
	void getModuleDataCSV(Robot* rob, std::string csv_path);
	
};