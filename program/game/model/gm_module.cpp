#include "gm_module.h"

void Module::update(float delta_time) {
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_o_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera* camera) {
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::DirectKinematics_world(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- ���^���w : ���[���h���W�n��o��Ɍv�Z---- //
	// �t�^���w�v�Z����Ver.
	// --- �ʒu�E��]���X�V ---
	pos_o_ = p_back;
	rot_axis_ = tnl::Vector3::TransformCoord(rot_axis_, q_back);
	rot_axis_.normalize();		// �ۂߌ덷�΍�F���K��
	// --- IK�����F����]�̃N�H�[�^�j�I������
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rot_axis_, 0);
	rot_tmp_ = iqi * q_back;		// �{���W���[���̋Ǐ��I�ȉ�]�𐶐�
	rot_sum_ *= rot_tmp_;		// �����p������ǂꂾ����]���������i�[
	// --- �����x�N�g���X�V ---
	dir_z_ = tnl::Vector3::TransformCoord(dir_z_, rot_tmp_);
	dir_z_.normalize();			// �ۂߌ덷�΍�F���K��
	dir_x_ = tnl::Vector3::TransformCoord(dir_x_, rot_tmp_);
	dir_x_.normalize();			// �ۂߌ덷�΍�F���K��
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// �����W���[���̈ʒu���i�[ 
}

void Module::DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- ���^���w�F���[���h���W�n��o��Ɍv�Z ---- //
	// �t�^���w�v�Z�L��Ver.
	// --- �ʒu�E��]���X�V ---
	pos_o_ = p_back;
	rot_axis_ = tnl::Vector3::TransformCoord(rot_axis_, q_back);
	rot_axis_.normalize();		// �ۂߌ덷�΍�F���K��
	// --- IK�L��F�ʒu or �p���̃N�H�[�^�j�I������
	tnl::Quaternion iqi = InverseKinematics(delta_time);
	rot_tmp_ = iqi * q_back;
	rot_sum_ *= rot_tmp_;
	// --- �����x�N�g���X�V --- 
	dir_z_ = tnl::Vector3::TransformCoord(dir_z_, rot_tmp_);
	dir_z_.normalize();			// �ۂߌ덷�΍�F���K��
	dir_x_ = tnl::Vector3::TransformCoord(dir_x_, rot_tmp_);
	dir_x_.normalize();			// �ۂߌ덷�΍�F���K��
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// �����W���[���̈ʒu���i�[ 
}

tnl::Quaternion Module::InverseKinematics(float delta_time) {
	// ---- �t�^���w�v�Z�F���[���h���W�n��o�
	// cnt_objects��cnt_target�̏�Ԃ֖{���W���[����]������ŋ߂Â��悤IK���v�Z
	// ��]������̔����p�xdth�̓t���[�����[�g�ω��ŋ��������Ȃ��Ȃ��悤�A���ݎ��Ԃ��悶�Ă���(�W��60fps�z��)
	// <�p�����[�^>
	// pos : �ʒu�֋߂Â���IK�v�Z
	// rotz : �p��z �����֋߂Â���IK�v�Z
	// rotx : �p��x �����֋߂Â���IK�v�Z
	// --- �����p�x�v�Z ---
	float dth = 0;
	float dth_sum = 0;
	if (is_posIK) {
		for (int i = 0; i < kp_p_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->pos_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->pos_), rot_axis_);
			dth = delta_time * 60.0 * kp_p_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// ��]�����cnt_target : ���ٓ_�Fdth = 0
			// ��]��������
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}

	if (is_dir_zIK) {
		for (int i = 0; i < kp_rz_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->dir_z_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->dir_z_), rot_axis_);
			dth = delta_time * 60.0 * kp_rz_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// ��]�����cnt_target : ���ٓ_�Fdth = 0
			// ��]��������
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}

	if (is_dir_xIK) {
		for (int i = 0; i < kp_rx_nums_.size(); i++) {
			tnl::Vector3 x = tnl::Vector3::Cross((cnt_objects_[i]->dir_x_), rot_axis_);
			tnl::Vector3 y = tnl::Vector3::Cross((cnt_targets_[i]->dir_x_), rot_axis_);
			dth = delta_time * 60.0 * kp_rx_nums_[i] * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// ��]�����cnt_target : ���ٓ_�Fdth = 0
			// ��]��������
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();
			dth *= rot_axis_.dot(axis) > 0 ? 1 : -1;
			dth_sum += dth;
		}
	}
	// ��]�ʂ�Ԃ�
	return tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
}

void Module::UpdateCnt_obj(FaceVec& cnt_object) {
	cnt_object.pos_ = pos_o_;
	cnt_object.dir_z_ = dir_z_;
	cnt_object.dir_x_ = dir_x_;
	cnt_object.rot_sum_ = rot_sum_;
	cnt_object.update(0);
}