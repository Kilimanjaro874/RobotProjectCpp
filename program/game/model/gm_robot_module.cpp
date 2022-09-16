#include "gm_robot_module.h"

MdlRobotModule* MdlRobotModule::Create() {

	// ----- ���W���[���̏����ݒ� ----- //
	MdlRobotModule* mdl = new MdlRobotModule();
	mdl->parts_.resize(e_parts_max);

	// ----- ���W���[����DK�p�����[�^������ ----- //
	mdl->rotAi_ = tnl::Vector3{ 0, 0, 1 };			// ���W���[���̉�]���x�N�g���ݒ�
	mdl->posLi_ = tnl::Vector3{ 0, 20, 0 };			// �{���W���[�����玟���W���[���ւ̋����x�N�g��

	// ---- ���W���[���̃����N01�p�[�c�ݒ� ---- //
	Parts* link = new Parts();
	link->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- �����ʒu���p���ύX ---
	link->ofs_pos_ += mdl->posLi_ / 2;
	
	// --- ���W���[���ɓo�^ --- 
	mdl->parts_[e_link01] = link;

	// ---- ���W���[���̉�]��01�p�[�c�ݒ� ---- //
	Parts* axis01 = new Parts();
	axis01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- �����ʒu���p���ύX ---
	axis01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	
	// --- ���W���[���ɓo�^ ---
	mdl->parts_[e_axis01] = axis01;

	// ---- ���W���[���̉�]��02�p�[�c�ݒ� ---- //
	Parts* axis02 = new Parts();
	axis02->mesh_ = dxe::Mesh::CreateDisk(5);
	axis02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- �����ʒu���p���ύX
	axis02->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	
	// --- ���W���[���ɓo�^ --- 
	mdl->parts_[e_axis02] = axis02;

	// ---- ���W���[���̉�]��03�p�[�c�ݒ� ---- //
	Parts* axis03 = new Parts();
	axis03->mesh_ = dxe::Mesh::CreateDisk(5);
	axis03->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// --- �����ʒu���p���ύX
	axis02->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(180));
	axis03->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };
	
	// --- ���W���[���ɓo�^ --- 
	mdl->parts_[e_axis03] = axis03;

	return mdl;
}

void MdlRobotModule::update(float delta_time) {
	// ----- ���N���X�F���f���̃A�b�v�f�[�g ----- //
	//Model::update(delta_time);
	// ----- �{���{�b�g���W���[�����L�̏����L�q ----- //
	for (auto pts : parts_) {
		/*pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_);
		pts->mesh_->rot_q_ *= rot_;*/
		//pts->mesh_->rot_q_ *= pts->ofs_rot_;
	}
}

void MdlRobotModule::localDK(tnl::Quaternion q_back, tnl::Vector3 l_back) {
	// ----- �Ǐ����^�����{ ----- //
	// ---- o��i-1�̏��Ŗ{���W���[���̈ʒu�E��]���x�N�g�����̏��X�V ----
	pos_ = tnl::Vector3::TransformCoord(pos_, q_back) + l_back;
	
	
	rotAi_ = tnl::Vector3::TransformCoord(rotAi_, q_back);
	// ---- �e�X�g�Fi��i�̔�����]�p�x����p������ ---- 
	float dth =	90;	// deg
	// ---- �Ǐ��I��(i��i)��]�N�H�[�^�j�I��iqi, ���s�ړ���iLi�v�Z
	tnl::Quaternion iqi = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(dth));
	tnl::Vector3 iLi = pos_ - tnl::Vector3::TransformCoord(pos_, iqi);
	// ---- ��0��̉�]�N�H�[�^�j�I��oqi, ���s�ړ���oLi�v�Z
	tnl::Quaternion oqi = iqi * q_back;
	rot_ = oqi;
	
	rot_total *= oqi;	// ��]�ʐώZ�l
	
	tnl::Vector3 oLi = tnl::Vector3::TransformCoord(l_back, iqi) + iLi;
	oLi_ = oLi;
	// ---- �������Ă���p�[�c�̏��X�V ---- 
	for (auto pts : parts_) {
		
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_total);
		pts->mesh_->rot_q_ = pts->ofs_rot_* rot_total;
	}

}
