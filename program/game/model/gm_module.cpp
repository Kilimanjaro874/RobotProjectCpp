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
	rot_sum_ = rot_sum_ * q_back;
	rot_axis_ = tnl::Vector3::TransformCoord(init_rot_axis_, rot_sum_);
	rot_tmp_ = tnl::Quaternion::RotationAxis(rot_axis_, 0);
	rot_sum_ = rot_sum_ * rot_tmp_;
	dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	dir_x_ = tnl::Vector3::TransformCoord(init_dir_x_, rot_sum_);
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// �����W���[���̈ʒu���i�[
	
}

void Module::DirectKinematicsWithIK_world(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
	// ---- ���^���w�F���[���h���W�n��o��Ɍv�Z ---- //
	// �t�^���w�v�Z�L��Ver.
	// --- �ʒu�E��]���X�V ---
	pos_o_ = p_back;
	rot_sum_ *= q_back;
	rot_axis_ = tnl::Vector3::TransformCoord(init_rot_axis_, rot_sum_);
	rot_axis_.normalize();
	tnl::Quaternion iqi = InverseKinematics(delta_time);
	rot_tmp_ = q_back * iqi;
	rot_sum_ *= iqi;
	dir_z_ = tnl::Vector3::TransformCoord(init_dir_z_, rot_sum_);
	dir_z_.normalize();			// �ۂߌ덷�΍�F���K��
	dir_x_ = tnl::Vector3::TransformCoord(init_dir_x_, rot_sum_);
	dir_x_.normalize();			// �ۂߌ덷�΍�F���K��
	pos_o_next_ = pos_o_ + dir_z_ * link_length_;	// �����W���[���̈ʒu���i�[
}

tnl::Quaternion Module::InverseKinematics(float delta_time) {
	// ---- �t�^���w�v�Z�F���[���h���W�n��o�
	// ik_setting�̒l�ɉ�����target, object�ŉ���IK�̎�ނ��ω�����
	// ��]������̔����p�xdth�̓t���[�����[�g�ω��ŋ��������Ȃ��Ȃ��悤�A���ݎ��Ԃ��悶�Ă���(�W��60fps�z��)
	// <�p�����[�^>
	
	// --- �����p�x�v�Z ---
	float dth = 0;
	float dth_sum = 0;
	tnl::Vector3 tmp_pe;
	tnl::Vector3 tmp_pr;
	tnl::Vector3 x;
	tnl::Vector3 y;
	for (int i = 0; i < ik_settings_.size(); i++) {
		switch (ik_settings_[i].ik_type)
		{
		case(pos_to_pos):
			tmp_pe = ik_settings_[i].object->pos_ - pos_o_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirz_to_pos):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirx_to_pos):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->pos_ - pos_o_;
			break;
		case(dirz_to_dirz):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->dir_z_;
			break;
		case(dirx_to_dirx):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->dir_x_;
			break;
		case(dirz_to_dirx):
			tmp_pe = ik_settings_[i].object->dir_z_;
			tmp_pr = ik_settings_[i].target->dir_x_;
			break;
		case(dirx_to_dirz):
			tmp_pe = ik_settings_[i].object->dir_x_;
			tmp_pr = ik_settings_[i].target->dir_z_;
			break;

		default:
			printf("error: ik_type�̐ݒ���������Ă�������");
			break;
		}
		// --- IK�����J�n --- //
		tnl::Vector3 x = tnl::Vector3::Cross(tmp_pe, rot_axis_);
		tnl::Vector3 y = tnl::Vector3::Cross(tmp_pr, rot_axis_);
		dth = delta_time * 60.0 * ik_settings_[i].kp * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// ��]�����target or object : ���ٓ_->dth = 0�ŃG���[���
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// ��]��������
		dth *= rot_axis_.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	
	// ��]�ʂ��N�H�[�^�j�I���ŕԂ�
	return tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
}

