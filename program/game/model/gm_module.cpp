#include "gm_module.h"

void Module::partsUpdate(float delta_time) {
	// ---- ���i�̈ʒu�E�p�������W���[�����W�n�ɉ����čX�V ---- //
	for (auto pts : _parts) {
		pts->mesh_->pos_ = _pos + tnl::Vector3::TransformCoord(pts->ofs_pos_, _rot);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * _rot;
	}
}

void Module::partsRender(dxe::Camera* camera) {
	// ----- ���i�̃V�F�[�_�[��`�悷�� ----- //
	if (!_is_render) { return; }
	for (auto pts : _parts) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::partsUpdateTree(const Module* mod, float delta_time) {
	// ---- partsUpdate���q���W���[���ɍċA�I�Ɏ��{ : preorder ---- //
	partsUpdate(delta_time);
	if (_children.size() == 0) return;		// �q�������F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsUpdateTree(_children[i], delta_time);	// �q�̊֐����{
	}
}

void Module::partsRenderTree(const Module* mod, dxe::Camera* camera) {
	// ---- partsRender���q���W���[���ɍċA�I�Ɏ��{�Fpreorder ---- //
	partsRender(camera);
	if (_children.size() == 0) return;		// �q���Ȃ��F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsRenderTree(_children[i], camera);		// �q�̊֐����{
	}
}

Module* Module::createModule(int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot , tnl::Vector3 dir_z, tnl::Vector3 dir_x) {
	// ----- ���W���[���𐶐����� ----- //
	Module* mod = new Module();
	mod->_id = id;
	mod->_name = name;
	mod->_pos = pos;
	mod->_rot_axis = rot_axis;
	mod->_rot = rot;
	mod->_dir_z = dir_z;
	mod->_dir_x = dir_x;

	return mod;
}

void Module::attachModule(Module* parent, Module* child, _attach_type type) {
	// ----- ���W���[���c���[�\���\�z�̂��߁A�e�q�֌W�ݒ� ----- //
	parent->_children.push_back(child);
	child->_parent = parent;
	// ---- �΍��x�N�g����e�Ɋi�[ ---- //
	tnl::Vector3 delta_pos;
	if (_attach_type::absolute == type) { delta_pos = child->_pos - parent->_pos; }
	else if (_attach_type::relative == type)	{ delta_pos = child->_pos; }
	float delta_length = delta_pos.length();
	delta_pos.normalize();
	// ---- �\���̂Ɋi�[ ---- //
	parent->_dk_st.push_back({ child->_id, child->_name, 
		delta_pos, delta_length, tnl::Quaternion::RotationAxis({0, 1, 0}, 0) });
}

void Module::directKinematics(const std::vector<dk_st>& dk) {
	// ----- ��ɐe���W���[����dk_st���Q�Ƃ��A���g�̍��W�n�E�p���X�V ----- //
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- id����v�������̏����F���W�n�X�V ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);
	}
	// ---- �q���W���[���܂ł̑��΍��W���X�V ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
		_dk_st_tmp[i]._rot_sum = _rot_tmp;
	}
	// ---- �q���W���[���܂ł̐�΍��W���X�V�F�q���W���[���ɓn���̂͂����� ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}
}

void Module::directKinematicsTree(const Module* mod, std::vector<dk_st>& dk) {
	// ----- directKinematics���q���W���[���ɍċA�I�Ɏ��{�Fpreorder ----- //
	directKinematics(dk);
	if (_children.size() == 0) return;		// �q���Ȃ��F�e���֖߂�
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsTree(_children[i], this->_dk_st_next);		// �q�̊֐����{
	}
}
