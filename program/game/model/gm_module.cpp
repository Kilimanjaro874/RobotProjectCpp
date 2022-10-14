#include "gm_module.h"

void Module::update(float delta_time) {
	// ---- ���i�ʒu�E�p���X�V ---- //
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_o_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_sum_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_sum_;
	}
}

void Module::render(dxe::Camera* camera) {
	// ---- ���i�`�� ---- //
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}


void Module::InitDK(const std::vector<dk_setting>& dks) {
	// ---- ���^���w�̂��߂̏��������� ---- //
	// --- ���W�n������ --- //
	bool do_DK = false;
	for (auto d : dks) {
		if (d.id_ == id_) {
			pos_o_ = d.dir_r_n_ * d.dir_r_length_;				// �ʒu
			rot_sum_ *= d.q_r_n_;								// �p��
			do_DK = true;
			break;
		}
	}
	if (!do_DK) { return; }		// DK�̑ΏۊO�ł���Ί֐��𔲂���.
	in_rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);
	in_rot_axis_.normalize();		// ��]��
	in_dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	in_dir_z_.normalize();			// ���W���[��z�������P�ʃx�N�g��(���W�n�Ƃ��Ďg�p)
	dir_z_ = in_dir_z_;
	in_dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	in_dir_x_.normalize();			// ���W���[��x�������P�ʃx�N�g��(���W�n�Ƃ��Ďg�p)
	dir_x_ = in_dir_x_;
	// --- DK�Z�b�e�B���O������ --- //
	for (auto dks : dk_s_v_) {
		dks.dir_r_length_ = dks.dir_r_n_.length();	// �����W���[���܂ł̋����i�[
		dks.dir_r_n_.normalize();
		dks.dir_r_n_ = tnl::Vector3::TransformCoord(dks.dir_r_n_, rot_sum_);
		dks.dir_r_n_.normalize();					// �����W���[���܂ł̕����P�ʃx�N�g���i�[
	}
}

void Module::SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ---- ���^���w�̂��߂̏����F���g�̈ʒu�E�p���������������ɍX�V ---- //
	pos_o_ = pos;
	rot_sum_ *= rot;
	in_rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);
	in_rot_axis_.normalize();		// ��]��
	in_dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);
	in_dir_z_.normalize();			// ���W���[��z�������P�ʃx�N�g��(���W�n�Ƃ��Ďg�p)
	dir_z_ = in_dir_z_;
	in_dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);
	in_dir_x_.normalize();			// ���W���[��x�������P�ʃx�N�g��(���W�n�Ƃ��Ďg�p)
	dir_x_ = in_dir_x_;
}

void Module::DKwithIK(float delta_time, const std::vector<dk_setting>& dks) {
	// ---- ���^���w���t�^���w�v�Z ---- //
	// --- ���W�n������ --- //
	bool do_DK = false;
	for (auto d : dks) {
		if (d.id_ == id_) {
			pos_o_ = d.dir_r_n_ * d.dir_r_length_;				// �ʒu
			rot_sum_ *= d.q_r_n_;
			rot_tmp_ = d.q_r_n_;
			do_DK = true;
			break;
		}
	}
	if (!do_DK) { return; }		// DK�̑ΏۊO�ł���Ί֐��𔲂���.
	rot_axis_ = tnl::Vector3::TransformCoord(in_rot_axis_, rot_sum_);	// ��]��
	// --- IK���{ --- //
	if (ik_s_v_.size() == 0) { return; }	// IK�����{���Ȃ��Ȃ�΁A�֐��𔲂���.
	float dth = 0;
	float dth_sum = 0;
	tnl::Vector3 tmp_pe;	// ����Ώۈʒu or �p���x�N�g��
	tnl::Vector3 tmp_pr;	// ����ڕW�ʒu or �p���x�N�g��
	tnl::Vector3 x;			// �v�Z���ʈꎞ�i�[�p
	tnl::Vector3 y;			// �v�Z���ʈꎞ�i�[�p
	for (int i = 0; i < ik_s_v_.size(); i++) {
		switch (ik_s_v_[i].ik_type_)
		{
		case(pos_to_pos):
			tmp_pe = ik_s_v_[i].object_->pos_o_ - pos_o_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirz_to_pos):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirx_to_pos):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->pos_o_ - pos_o_;
			break;
		case(dirz_to_dirz):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->dir_z_;
			break;
		case(dirx_to_dirx):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->dir_x_;
			break;
		case(dirz_to_dirx):
			tmp_pe = ik_s_v_[i].object_->dir_z_;
			tmp_pr = ik_s_v_[i].target_->dir_x_;
			break;
		case(dirx_to_dirz):
			tmp_pe = ik_s_v_[i].object_->dir_x_;
			tmp_pr = ik_s_v_[i].target_->dir_z_;
			break;

		default:
			printf("error: ik_type�̐ݒ���������Ă�������");
			break;
		}
		tnl::Vector3 x = tnl::Vector3::Cross(tmp_pe, rot_axis_);
		tnl::Vector3 y = tnl::Vector3::Cross(tmp_pr, rot_axis_);
		dth = delta_time * 60.0 * ik_s_v_[i].kp_ * std::acosf(std::clamp(
			x.dot(y) / x.length() / y.length(),
			(float)-1, (float)1
		));
		if (!isfinite(dth)) { dth = 0; }	// ��]�����target or object : ���ٓ_->dth = 0�ŃG���[���
		if (dth > tnl::PI / 24) {
			dth = tnl::PI / 24;
		}
		tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// ��]��������
		dth *= rot_axis_.dot(axis) >= 0 ? 1 : -1;
		dth_sum += dth;
	}
	tnl::Quaternion tmp_rot = tnl::Quaternion::RotationAxis(rot_axis_, dth_sum);
	// --- DK���{ --- //
	rot_tmp_ *= tmp_rot;
	rot_sum_ *= tmp_rot;
	dir_z_ = tnl::Vector3::TransformCoord(in_dir_z_, rot_sum_);	// z��
	dir_x_ = tnl::Vector3::TransformCoord(in_dir_x_, rot_sum_);	// x��
	// --- DK_setting�X�V --- //
	for (auto dks : dk_s_v_) {
		dks.q_r_n_ = rot_tmp_;
		dks.dir_r_n_ = tnl::Vector3::TransformCoord(dks.dir_r_n_, rot_tmp_);
		dks.dir_r_n_.normalize();
	}
}