#include "gm_module.h"

void Module::partsUpdate(float delta_time) {
	// ---- 部品の位置・姿勢をモジュール座標系に応じて更新 ---- //
	for (auto pts : _parts) {
		pts->mesh_->pos_ = _pos + tnl::Vector3::TransformCoord(pts->ofs_pos_, _rot);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * _rot;
	}
}

void Module::partsRender(dxe::Camera* camera) {
	// ----- 部品のシェーダーを描画する ----- //
	if (!_is_render) { return; }
	for (auto pts : _parts) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

void Module::partsUpdateTree(const Module* mod, float delta_time) {
	// ---- partsUpdateを子モジュールに再帰的に実施 : preorder ---- //
	partsUpdate(delta_time);
	if (_children.size() == 0) return;		// 子が無い：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsUpdateTree(_children[i], delta_time);	// 子の関数実施
	}
}

void Module::partsRenderTree(const Module* mod, dxe::Camera* camera) {
	// ---- partsRenderを子モジュールに再帰的に実施：preorder ---- //
	partsRender(camera);
	if (_children.size() == 0) return;		// 子がない：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->partsRenderTree(_children[i], camera);		// 子の関数実施
	}
}

Module* Module::createModule(int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot , tnl::Vector3 dir_z, tnl::Vector3 dir_x) {
	// ----- モジュールを生成する ----- //
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
	// ----- モジュールツリー構造構築のため、親子関係設定 ----- //
	parent->_children.push_back(child);
	child->_parent = parent;
	// ---- 偏差ベクトルを親に格納 ---- //
	tnl::Vector3 delta_pos;
	if (_attach_type::absolute == type) { delta_pos = child->_pos - parent->_pos; }
	else if (_attach_type::relative == type)	{ delta_pos = child->_pos; }
	float delta_length = delta_pos.length();
	delta_pos.normalize();
	// ---- 構造体に格納 ---- //
	parent->_dk_st.push_back({ child->_id, child->_name, 
		delta_pos, delta_length, tnl::Quaternion::RotationAxis({0, 1, 0}, 0) });
}

void Module::directKinematics(const std::vector<dk_st>& dk) {
	// ----- 主に親モジュールのdk_stを参照し、自身の座標系・姿勢更新 ----- //
	for (auto d : dk) {
		if (_id != d._id) { continue; }
		// ---- idが一致した時の処理：座標系更新 ---- //
		_pos = d._dir * d._length;
		_rot *= d._rot_sum;
		_rot_tmp = d._rot_sum;
		_rot_axis_tmp = tnl::Vector3::TransformCoord(_rot_axis, _rot);
		_dir_z_tmp = tnl::Vector3::TransformCoord(_dir_z, _rot);
		_dir_x_tmp = tnl::Vector3::TransformCoord(_dir_x, _rot);
	}
	// ---- 子モジュールまでの相対座標を更新 ---- //
	_dk_st_tmp = _dk_st;
	for (int i = 0; i < _dk_st.size(); i++) {
		_dk_st_tmp[i]._dir = tnl::Vector3::TransformCoord(_dk_st[i]._dir, _rot);
		_dk_st_tmp[i]._rot_sum = _rot_tmp;
	}
	// ---- 子モジュールまでの絶対座標を更新：子モジュールに渡すのはこちら ---- //
	_dk_st_next = _dk_st_tmp;
	for (int i = 0; i < _dk_st_next.size(); i++) {
		_dk_st_next[i]._dir = _pos + _dk_st[i]._dir * _dk_st[i]._length;
		_dk_st_next[i]._length = _dk_st_next[i]._dir.length();
		_dk_st_next[i]._dir.normalize();
	}
}

void Module::directKinematicsTree(const Module* mod, std::vector<dk_st>& dk) {
	// ----- directKinematicsを子モジュールに再帰的に実施：preorder ----- //
	directKinematics(dk);
	if (_children.size() == 0) return;		// 子がない：親側へ戻る
	for (int i = 0; i < _children.size(); i++) {
		_children[i]->directKinematicsTree(_children[i], this->_dk_st_next);		// 子の関数実施
	}
}
