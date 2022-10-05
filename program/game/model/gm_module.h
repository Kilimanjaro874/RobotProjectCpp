#pragma once
#include "gm_parts.h"
#include "gm_faceVec.h"

class Module {
public:
	Module() {};
	virtual ~Module() {
		for (auto pts : parts_) delete pts;
	}
	int id_;						// �Q�Ɨp
	// ---- Direct Kinematics : DK�ɕK�v�ȕϐ� ---- //
	tnl::Vector3 pos_a_;			// �G�[�W�F���g���W�nx��ai����̖{���W���[�����W�n���_�ʒu
	tnl::Vector3 pos_o_;			// ���[���h���Wo������̖{���W���[��i�̍��W�n���_�ʒu
	tnl::Vector3 pos_a_next_;		// �G�[�W�F���g���W�n�F�����W���[���̈ʒu���i�[
	tnl::Vector3 pos_o_next_;		// ���[���h���W�n�F�����W���[���̈ʒu���i�[
	tnl::Vector3 dir_z_;			// �{���W���[��i���烂�W���[��i+i�ւ̕����P�ʃx�N�g�����`(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 init_dir_z_;		// �{���W���[��i���烂�W���[��i+i�ւ̕����P�ʃx�N�g�����`�E�������(�ʒuIK�v�Z�Ɏg�p)
	tnl::Vector3 dir_x_;			// �{���W���[��i�p������̂��߂̎p�������P�ʃx�N�g�����`(�p��IK�v�Z�Ɏg�p)
	tnl::Vector3 init_dir_x_;
	tnl::Vector3 rot_axis_;			// ���W���[��i�̉�]���P�ʃx�N�g���F�{���W���[�����_�ɗ��Ă鎖�Ƃ���
	tnl::Vector3 init_rot_axis_;
	float link_length_;				// dir_z_�̑傫�����i�[�F���W���[��i���_���烂�W���[��i+i�܂ł̒������`
	tnl::Quaternion rot_tmp_;		// IK�v�Z���Ao����ɋL�q���ꂽ�A1�t���[���ԂŖ{���W���[�������ꎞ�I�ȉ�]�ʂ��i�[
	tnl::Quaternion rot_sum_;		// �{���W���[���������p������ǂꂾ����]���������i�[���Ă����B
	std::vector<float> kp_p_nums_;	// IK�F�����p�x�v�Z�̍ۂɎg�p������萔 ���ڕW�ʒu�Q�Ɨp�F��������ꍇ�ɂ��Ή��̂���vector�\�� 
	std::vector<float> kp_rz_nums_;	// IK�F�����p�x�v�Z�̍ۂɎg�p������萔 ���ڕW�p��z�Q�Ɨp�F��������ꍇ�ɂ��Ή��̂���vector�\��
	std::vector<float> kp_rx_nums_;	// IK�F�����p�x�v�Z�̍ۂɎg�p������萔 ���ڕW�p��x�Q�Ɨp�F��������ꍇ�ɂ��Ή��̂���vector�\��
	std::vector<FaceVec*> cnt_objects_;		// �L��x�N�g���N���X�F����Ώۂ����W���[���Ɏw��
	std::vector<FaceVec*> cnt_targets_;		// �L��x�N�g���N���X�F����ڕW�����W���[���Ɏw��
	bool is_posIK = false;			// IK : �ʒu�Q�Ƃŉ������ۂ�
	bool is_dir_zIK = false;		// IK : �p��z�Q�Ƃŉ������ۂ�
	bool is_dir_xIK = false;		// IK : �p��x�Q�Ƃŉ������ۂ�
	// ---- Render ----- //
	std::vector<Parts*> parts_;
	// ---- �����o�֐� ---- //
	void update(float delta_time);
	void render(dxe::Camera* camera);
	void DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back);
	tnl::Quaternion InverseKinematics(float delta_time);
	void DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, 
		const tnl::Quaternion& q_back);
	//void SetCnt_obj(FaceVec& cnt_object);
	void Clear_kp_IKstate();
	void UpdateCnt_obj(FaceVec& cnt_object);

};