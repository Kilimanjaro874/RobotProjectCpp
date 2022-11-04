#include "gm_robot.h"

Robot* Robot::Create(const tnl::Vector3 pos, const tnl::Quaternion rot) {
	// ----- �ʒu pos, �p�� rot �փ��{�b�g�𐶐� ----- //
	Robot* rob = new Robot();
	// ---- rob : �v���C���[�̑�����󂯕t����B���̂��߂̏��������{ ---- //
	rob->init(rob, 1, "rob_ref_coord", pos, { 0, 1, 0 }, rot);
	// test : �l�p�`���A�^�b�`���� 
	Module* box = Module::createModule(2, "box1", { 0, 5, 0 }, { 0, 1, 0 }, 
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(0)));
	box->setAxisView(0.3, 5);
	Parts* box_s = new Parts();
	box_s->mesh_ = dxe::Mesh::CreateBox(3);
	box_s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	box->_parts.push_back(box_s);

	rob->attachModule(rob, box);

	// test : �X�Ɏl�p�`���A�^�b�`����
	Module* box2 = Module::createModule(3, "box2", { 0, 15, 0 }, { 0, 1, 0 },
		tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(-45)));
	box2->setAxisView(0.3, 5);
	Parts* box_2s = new Parts();
	box_2s->mesh_ = dxe::Mesh::CreateBox(3);
	box_2s->mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	box2->_parts.push_back(box_2s);

	box->attachModule(box, box2);

	return rob;

}

void Robot::init(Robot* rob, int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis, tnl::Quaternion rot) {
	// ---- rob : �v���C���[�̑�����󂯕t����B���̂��߂̏��������{ ---- //
	rob->_id = id;
	rob->_name = name;
	rob->_pos = pos;
	rob->_rot_axis = rot_axis;
	rob->_rot = rot;
	// --- ���W�n�̏����� --- //
	rob->_dir_z = { 0, 0, 1 };
	rob->_dir_x = { 1, 0, 0 };
	// --- DK�p�����[�^�̏����� --- //
	rob->_dk_input.resize(1);
	rob->_dk_input[0]._id = rob->_id;
	rob->_dk_input[0]._name = rob->_name;
	rob->_dk_st = rob->_dk_input;
}

