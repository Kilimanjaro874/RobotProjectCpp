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

void Module::InitParams(int id, tnl::Vector3 rot_axis, tnl::Quaternion rot_sum, 
	tnl::Vector3 dir_z, tnl::Vector3 dir_x){
	// ---- ���W���[���p�����[�^�������̂��߂Ɏg�p ---- //
	id_ = id;
	in_rot_axis_ = rot_axis;
	rot_sum = rot_sum;
	in_dir_z_ = dir_z;
	in_dir_x_ = dir_x;
}

void Module::InitDK(const std::vector<dk_setting>& dks) {
	// ---- ���^���w�̂��߂̏��������� ---- //
	bool do_DK = false;
	for (auto d : dks) {
		if (d.id_ == id_) {
			// �{���W���[���ʒu�E�p���E���W�n�X�V
			SelfDK(d.dir_r_n_ * d.dir_r_length_, d.q_r_n_);
			do_DK = true;
		}
	}
	if (!do_DK) { return; }		// DK�̑ΏۊO�ł���Ί֐��𔲂���.
	//�@���̃��W���[���ʒu�E�p�����X�V���邽�߂�DK�p�����[�^������
	for (auto d_ : dk_s_v_) {
		// �x�N�g����P�ʃx�N�g���i����)�A�����ɕ����B���l�v�Z�݌v�덷��������
		d_.dir_r_length_ = d_.dir_r_n_.length();	// �����W���[���܂ł̋����i�[
		d_.dir_r_n_.normalize();
		d_.q_r_n_ *= rot_tmp_;					// �����W���[���ɖ{���W���[���܂ł̉�]�ʗ݌v�`�B
	}
	// DK�p�����[�^�����l�쐬
	in_dk_s_v_.resize(dk_s_v_.size());
	in_dk_s_v_ = dk_s_v_;			// �����l�Ƃ��ăR�s�[�BUpdate()�ł́A�����W���[���̈ʒu�E�p����{�\���̂ŕ\�����X�V���Ă���
	// DK�p�����[�^�X�V�l�쐬
	for (int i = 0; i < in_dk_s_v_.size(); i++) {
		dk_s_v_[i].dir_r_n_ = pos_o_ + 
			tnl::Vector3::TransformCoord(in_dk_s_v_[i].dir_r_n_, in_dk_s_v_[i].q_r_n_) * in_dk_s_v_[i].dir_r_length_;
	}
}

void Module::SelfDK(const tnl::Vector3& pos, const tnl::Quaternion& rot) {
	// ---- ���^���w�̂��߂̏����F���g�̈ʒu�E�p���������������ɍX�V ---- //
	pos_o_ = pos;
	rot_sum_ *= rot;
	rot_tmp_ = rot;
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

void Module::AllInitDK(const Module* mod, const std::vector<dk_setting>& dks) {
	// --- �ċA�I��Module�̉^���w���s�B���{�b�g�����ʒu�E�p�������������� --- //
	// preorder�Ŏ��s���鎖
	InitDK(dks);
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		//AllInitDK(next[i], dk_s_v_);
		next[i]->AllInitDK(next[i], dk_s_v_);
	}
}

void Module::updateTree(const Module* mod, float delta_time) {
	// --- �o�^�������W���[���Q�� preorder�ōX�V���Ă��� --- //
	update(delta_time);
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->updateTree(next[i], delta_time);
	}

}

void Module::renderTree(const Module* mod, dxe::Camera* camera) {
	// --- �o�^�������W���[���Q�� preorder�ŕ`�悵�Ă��� --- //
	if (is_render_) { render(camera); }
	if (next.size() == 0) return;
	for (int i = 0; i < next.size(); i++) {
		next[i]->renderTree(next[i], camera);
	}
}