#include "gm_rb_arm_r.h"

MdlRbArmR* MdlRbArmR::Create(tnl::Vector3 p_back, tnl::Quaternion q_back) {
	// ----- �Ǘ����郂�W���[���쐬 ----- //
	MdlRbArmR* agn = new MdlRbArmR();
	agn->modules_.resize(e_modules_max);
	// �����ʒu�E�p��
	agn->pos_ = tnl::Vector3{ 0, 0, 0 };
	agn->rotAi_ = tnl::Vector3{ 1, 0, 0 };
	agn->link_dir = tnl::Vector3{ 1, 0, 0 };
	agn->link_length = 0;

	// ---- 1. e_sho_x ---- //
	Module* sho_x = new Module();
	// �����ʒu�E�p��
	sho_x->pos_ = tnl::Vector3{ 0, 0, 0 };
	sho_x->rotAi_ = tnl::Vector3{ 1, 0, 0 };
	sho_x->link_dir = tnl::Vector3{ 0, -1, 0 };
	sho_x->link_length = 0;
	// �^���w�v�Z�p�����[�^������
	sho_x->kp_num_.push_back(0.3);
	sho_x->kp_num_.push_back(0.5);
	// Parts�F���W���[���ɓo�^�F��
	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_sho_x] = sho_x;


	// ---- 2. e_sho_z ---- //
	Module* sho_z = new Module();
	// �����ʒu�E�p��
	sho_z->pos_ = tnl::Vector3{ 0, 0, 0 };
	sho_z->rotAi_ = tnl::Vector3{ 0, 0, 1 };
	sho_z->link_dir = tnl::Vector3{ 0, -1, 0 };
	sho_z->link_length = 20;
	// �^���w�v�Z�p�����[�^������
	sho_z->kp_num_.push_back(0.2);
	// Parts: ���W���[���ɓo�^
	Parts* sho_z_ln01 = new Parts();
	sho_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	sho_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_ln01->ofs_pos_ += sho_z->link_dir * sho_z->link_length / 2;
	Parts* sho_z_rot01 = new Parts();
	sho_z_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	sho_z_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* sho_z_lid01 = new Parts();
	sho_z_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_z_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* sho_z_lid02 = new Parts();
	sho_z_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	sho_z_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	sho_z_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	sho_z->parts_.push_back(sho_z_ln01);
	sho_z->parts_.push_back(sho_z_rot01);
	sho_z->parts_.push_back(sho_z_lid01);
	sho_z->parts_.push_back(sho_z_lid02);
	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_sho_z] = sho_z;


	// ---- 3. e_arm_x ---- //
	Module* arm_x = new Module();
	// �����ʒu�E�p��
	arm_x->pos_ = tnl::Vector3{ 0, -20, 0 };
	arm_x->rotAi_ = tnl::Vector3{ 1, 0, 0 };
	arm_x->link_dir = tnl::Vector3{ 0, -1, 0 };
	arm_x->link_length = 0;
	// �^���w�v�Z�p�����[�^������
	arm_x->kp_num_.push_back(0.2);
	// Parts�F���W���[���ɓo�^�F��
	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_arm_x] = arm_x;

	
	// ---- 4. e_arm_y ---- //
	Module* arm_y = new Module();
	// �����ʒu�E�p��
	arm_y->pos_ = tnl::Vector3{ 0, -20, 0 };
	arm_y->rotAi_ = tnl::Vector3{ 0, -1, 0 };
	arm_y->link_dir = tnl::Vector3{ 0, -1, 0 };
	arm_y->link_length = 20;
	// �^���w�v�Z�p�����[�^������
	arm_y->kp_num_.push_back(0.2);
	// Parts�F���W���[���ɓo�^
	Parts* arm_y_ln01 = new Parts();
	arm_y_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	arm_y_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_ln01->ofs_pos_ += arm_y->link_dir * arm_y->link_length / 2;
	Parts* arm_y_rot01 = new Parts();
	arm_y_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	arm_y_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* arm_y_lid01 = new Parts();
	arm_y_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_y_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* arm_y_lid02 = new Parts();
	arm_y_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	arm_y_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	arm_y_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	arm_y->parts_.push_back(arm_y_ln01);
	arm_y->parts_.push_back(arm_y_rot01);
	arm_y->parts_.push_back(arm_y_lid01);
	arm_y->parts_.push_back(arm_y_lid02);
	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_arm_y] = arm_y;
	

	// ---- 5. e_wrist_x ---- //
	Module* wrist_x = new Module();
	// �����ʒu�E�p��
	wrist_x->pos_ = tnl::Vector3{ 0, -40, 0 };
	wrist_x->rotAi_ = tnl::Vector3{ 1, 0, 0 };
	wrist_x->link_dir = tnl::Vector3{ 0, -1, 0 };
	wrist_x->link_length = 0;
	// �^���w�v�Z�p�����[�^������
	wrist_x->kp_num_.push_back(0.2);
	// Parts�F���W���[���ɓo�^�F��
	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_wrist_x] = wrist_x;


	// ---- 6. e_wrist_z ---- //
	Module* wrist_z = new Module();
	// �����ʒu�E�p��
	wrist_z->pos_ = tnl::Vector3{ 0, -40, 0 };
	wrist_z->rotAi_ = tnl::Vector3{ 0, 0, -1 };
	wrist_z->link_dir = tnl::Vector3{ 0, -1, 0 };
	wrist_z->link_length = 20;
	// �^���w�v�Z�p�����[�^������
	wrist_z->kp_num_.push_back(0.2);
	// Parts�F���W���[���ɓo�^
	Parts* wrist_z_ln01 = new Parts();
	wrist_z_ln01->mesh_ = dxe::Mesh::CreateCylinder(2.5, 20);
	wrist_z_ln01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wrist_z_ln01->ofs_pos_ += wrist_z->link_dir * wrist_z->link_length / 2;
	Parts* wrist_z_rot01 = new Parts();
	wrist_z_rot01->mesh_ = dxe::Mesh::CreateCylinder(5, 5);
	wrist_z_rot01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wrist_z_rot01->ofs_rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	Parts* wrist_z_lid01 = new Parts();
	wrist_z_lid01->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z_lid01->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wrist_z_lid01->ofs_pos_ += tnl::Vector3{ 0, 0, -2.5 };
	Parts* wrist_z_lid02 = new Parts();
	wrist_z_lid02->mesh_ = dxe::Mesh::CreateDisk(5);
	wrist_z_lid02->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	wrist_z_lid02->ofs_pos_ += tnl::Vector3{ 0, 0, 2.5 };

	wrist_z->parts_.push_back(wrist_z_ln01);
	wrist_z->parts_.push_back(wrist_z_rot01);
	wrist_z->parts_.push_back(wrist_z_lid01);
	wrist_z->parts_.push_back(wrist_z_lid02);

	// �G�[�W�F���g�ɓo�^
	agn->modules_[e_wrist_z] = wrist_z;

	// ----- ��L�����ʒu�E�p���𔽉f ----- //
	agn->localDK(p_back, q_back);	// �G�[�W�F���g���g�̍X�V
	tnl::Vector3 tmp_p_back = agn->pos_next;
	tnl::Quaternion tmp_q_back = agn->rot_;
	for (auto mod : agn->modules_) {
		mod->localDK(tmp_p_back, tmp_q_back);
		tmp_p_back = mod->pos_next;
		tmp_q_back = mod->rot_;
	}

	return agn;
}

void MdlRbArmR::update(float delta_time) {
	for (auto mod : modules_) {
		mod->update(delta_time);
	}
}

void MdlRbArmR::render(dxe::Camera* camera) {
	for (auto mod : modules_) {
		mod->render(camera);
	}
}

void MdlRbArmR::aimTarget(float delta_time, tnl::Vector3 p_back, tnl::Quaternion q_back, tnl::Vector3 target, tnl::Vector3 elbowRef) {
	// ----- �I�͎�O�Ɉ����Ȃ���A���̑��r�̓^�[�Q�b�g���G�C�������� ---- //
	// ---- �G�[�W�F���g�̍X�V ---- //
	localDK(p_back, q_back);
	pos_es_.clear();
	pos_es_.push_back(modules_[e_wrist_z]->pos_next);
	pos_es_.push_back(modules_[e_arm_x]->pos_);
	tnl::Vector3 tmp_p_back = pos_next;
	tnl::Quaternion tmp_q_back = rot_;
	// ---- ���W���[���̍X�V


	for (int i = 0; i < e_modules_max; i++) {
		// --- 1. e_sho_x --- //
		if (i == e_sho_x) {
			modules_[i]->pos_r_.clear();
			modules_[i]->pos_r_ = pos_rs_;
			modules_[i]->pos_e_.clear();
			modules_[i]->pos_e_ = pos_es_;
			modules_[i]->localDKwithLIK(delta_time, tmp_p_back, tmp_q_back);
			tmp_p_back = modules_[i]->pos_next;
			tmp_q_back = modules_[i]->rot_;
		}
		else if (i == e_sho_z) {
			modules_[i]->pos_r_.clear();
			modules_[i]->pos_r_.push_back(pos_rs_[1]);
			modules_[i]->pos_e_.clear();
			modules_[i]->pos_e_.push_back(pos_es_[1]);
			modules_[i]->localDKwithLIK(delta_time, tmp_p_back, tmp_q_back);
			tmp_p_back = modules_[i]->pos_next;
			tmp_q_back = modules_[i]->rot_;
		}
		else {
			modules_[i]->pos_r_.clear();
			modules_[i]->pos_r_.push_back(pos_rs_[0]);
			modules_[i]->pos_e_.clear();
			modules_[i]->pos_e_.push_back(pos_es_[0]);
			modules_[i]->localDKwithLIK(delta_time, tmp_p_back, tmp_q_back);
			tmp_p_back = modules_[i]->pos_next;
			tmp_q_back = modules_[i]->rot_;
		}

	}
}
