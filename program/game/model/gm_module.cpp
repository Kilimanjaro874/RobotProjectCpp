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



Module* Module::createModule(int id, std::string name, 
	tnl::Vector3 pos, tnl::Vector3 rot_axis,
	tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0),
	tnl::Vector3 dir_z = { 0, 0, 1 }, tnl::Vector3 dir_x = { 1, 0, 0 }) {
	// ----- モジュールを生成する ----- //
	Module* mod = new Module();
	mod->_id = id;
	mod->_name = name;
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
	if (_attach_type::absolute == type)			{ delta_pos = child->_pos - parent->_pos; }
	else if (_attach_type::relative == type)	{ delta_pos = child->_pos; }
	float delta_length = delta_pos.length();
	delta_pos.normalize();
	// --- 構造体に格納 --- //
	parent->_dk_st.push_back({ child->_id, child->_name, delta_pos, delta_length });
}

