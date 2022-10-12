#include "gm_ag_arm_r.h"

MdlArm_r* MdlArm_r::Create(tnl::Vector3& target_pos) {
	// ----- �Ǘ����郂�W���[���쐬 �J�n ----- //
	// ---- 0. ���g�F�G�[�W�F���g�쐬 ---- //
	MdlArm_r* agn = new MdlArm_r();
	agn->modules_.resize(e_models_max);
	// �ʒu�E�p���ݒ�
	agn->pos_ = tnl::Vector3{ 0, 0, 0 };
	agn->rotAi_ = tnl::Vector3{ 0, 1, 0 };	
	agn->link_dir = tnl::Vector3{ 0, 1, 0 };
	agn->link_length = 0;
	agn->target_pos_ = target_pos;

	// ---- 1. �����W���[���쐬 ---- //
	Module* shoulder = new Module();
	// �ʒu�E�p���ݒ�
	shoulder->pos_ = tnl::Vector3{ 0, 0, 0 };
	shoulder->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	shoulder->link_dir = tnl::Vector3{ 0, 1, 0 };
	shoulder->link_length = 20;
	shoulder->kp_ = 0.5;
	// --- �p�[�c��o�^���Ă��� ---
	// 1.0 : �����N
	Parts* link101 = new Parts();
	link101->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link101->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	link101->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
	// 1.1 : ��]���p�[�c1
	Parts* axis101 = new Parts();
	axis101->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis101->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis101->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : ��]���p�[�c2
	Parts* axis102 = new Parts();
	axis102->mesh_ = dxe::Mesh::CreateDisk(5);
	axis102->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis102->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : ��]���p�[�c3
	Parts* axis103 = new Parts();
	axis103->mesh_ = dxe::Mesh::CreateDisk(5);
	axis103->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis103->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- ���W���[���ɓo�^ ---
	shoulder->parts_.push_back(link101);
	shoulder->parts_.push_back(axis101);
	shoulder->parts_.push_back(axis102);
	shoulder->parts_.push_back(axis103);
	// --- �G�[�W�F���g�ɓo�^ ---
	agn->modules_[e_shoulder] = shoulder;


	// ---- 2, �r���W���[���쐬 ---- //
	Module* arm = new Module();
	// �ʒu�E�p���ݒ�
	arm->pos_ = tnl::Vector3{ 0, 20, 0 };
	arm->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	arm->link_dir = tnl::Vector3{ 0, 1, 0 };
	arm->link_length = 20;
	arm->kp_ = 0.4;
	// --- �p�[�c��o�^���Ă��� ---
	// 1.0 : �����N
	Parts* link201 = new Parts();
	link201->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link201->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	link201->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
	// 1.1 : ��]���p�[�c1
	Parts* axis201 = new Parts();
	axis201->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis201->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis201->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : ��]���p�[�c2
	Parts* axis202 = new Parts();
	axis202->mesh_ = dxe::Mesh::CreateDisk(5);
	axis202->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis202->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : ��]���p�[�c3
	Parts* axis203 = new Parts();
	axis203->mesh_ = dxe::Mesh::CreateDisk(5);
	axis203->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis203->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- ���W���[���ɓo�^ ---
	arm->parts_.push_back(link201);
	arm->parts_.push_back(axis201);
	arm->parts_.push_back(axis202);
	arm->parts_.push_back(axis203);
	// --- �G�[�W�F���g�ɓo�^ ---
	agn->modules_[e_arm] = arm;
	

	// ---- 3, ��񃂃W���[���쐬 ---- //
	Module* wrist = new Module();
	// �ʒu�E�p���ݒ�
	wrist->pos_ = tnl::Vector3{ 0, 40, 0 };
	wrist->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	wrist->link_dir = tnl::Vector3{ 0, 1, 0 };
	wrist->link_length = 20;
	wrist->kp_ = 0.2;
	// --- �p�[�c��o�^���Ă��� ---
	// 1.0 : �����N
	Parts* link301 = new Parts();
	link301->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	link301->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	link301->ofs_pos_ += tnl::Vector3{ 0, 10, 0 };
	// 1.1 : ��]���p�[�c1
	Parts* axis301 = new Parts();
	axis301->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	axis301->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis301->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	// 1.2 : ��]���p�[�c2
	Parts* axis302 = new Parts();
	axis302->mesh_ = dxe::Mesh::CreateDisk(5);
	axis302->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis302->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	// 1.3 : ��]���p�[�c3
	Parts* axis303 = new Parts();
	axis303->mesh_ = dxe::Mesh::CreateDisk(5);
	axis303->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	// �ʒu�E�p���ݒ�
	axis303->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	// --- ���W���[���ɓo�^ ---
	wrist->parts_.push_back(link301);
	wrist->parts_.push_back(axis301);
	wrist->parts_.push_back(axis302);
	wrist->parts_.push_back(axis303);
	// --- �G�[�W�F���g�ɓo�^ ---
	agn->modules_[e_wrist] = wrist;
	
	// ---- ��L�����ʒu���p���𔽉f ---- //
	agn->calcLDK(agn->pos_, tnl::Quaternion::RotationAxis(agn->rotAi_, 0));
	agn->update(0);	// ��L�����ʒu���p�����Q�[���`��O�ɓK��������B
	// ----- �Ǘ����郂�W���[���쐬 �I�� ----- //

	// ----- ���ڕW�ʒu�̃f�t�H���g�ݒ� 
	tnl::Vector3 tmp_pos_e = agn->modules_[agn->modules_.size()-1]->pos_next;
	agn->pos_rs_.resize(e_models_max);
	agn->pos_es_.resize(e_models_max);
	for (int i = 0; i < agn->modules_.size(); i++) {
		agn->pos_rs_[i] = target_pos;
		agn->pos_es_[i] = tmp_pos_e;
	}
	agn->target_es = tmp_pos_e;

	return agn;
}

void MdlArm_r::update(float delta_time) {
	for (auto mdl : modules_) {
		mdl->update(delta_time);
	}
}

void MdlArm_r::render(dxe::Camera* camera) {
	for (auto mdl : modules_) {
		mdl->render(camera);
	}
}


void MdlArm_r::testmove() {

	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		pos_ = forward_ * 10;
	}
	else if (tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
		pos_ = -forward_ * 10;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		tempQ_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);

	} else if(tnl::Input::IsKeyDown(eKeys::KB_LEFT)){
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(-3));
		forward_ = tnl::Vector3::TransformCoord(forward_, tempQ_);
	}
	else {
		rot_ = tnl::Quaternion::RotationAxis(rotAi_, tnl::ToRadian(0));
	}

}
//
//
//void MdlArm_r::calcLDK(const tnl::Vector3& p_back, const tnl::Quaternion& q_back) {
//	// ----- �O���G�[�W�F���g����q_back, l_back��Ⴂ�A�ێ����W���[���S�Ă�LDK�v�Z���{����֐� ----- //
//	// �t�^���w�v�Z�͂��Ȃ��B
//	
//	// ---- �G�[�W�F���g��LDK���s ----- //
//	this->localDK(p_back, q_back);
//	
//	// --- �ێ����W���[����LDK���s ---
//	tnl::Vector3* tmp_p_back = &this->pos_next;
//	tnl::Quaternion* tmp_q_back = &this->rot_;
//	for (auto mod : modules_) {
//		mod->localDK(*tmp_p_back, *tmp_q_back);
//		tmp_p_back = &mod->pos_next;
//		tmp_q_back = &mod->rot_;
//	}
//
//	tempQ_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
//}
//
//void MdlArm_r::calcLDKwithLIK(float delta_time, const tnl::Vector3& p_back, const tnl::Quaternion& q_back,
//	const tnl::Vector3& pos_e, const tnl::Vector3& pos_r) {
//
//	// ---- �G�[�W�F���g��LDK,LIK���s ---- //
//	this->localDKwithLIK(delta_time, p_back, q_back, pos_e, pos_r);
//	// --- �ێ����W���[����LDK��LIK���s ---
//	tnl::Vector3 tmp_p_back = this->pos_next;
//	tnl::Quaternion tmp_q_back = this->rot_;
//	for (int i = 0; i < modules_.size(); i++) {
//		modules_[i]->localDKwithLIK(delta_time, tmp_p_back, tmp_q_back, pos_es_[i], pos_rs_[i]);
//		tmp_p_back = modules_[i]->pos_next;
//		tmp_q_back = modules_[i]->rot_;
//		this->target_es = modules_[i]->pos_next;
//	}
//}