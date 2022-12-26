#include "gm_module.h"

/// <summary>
/// Init process : 
/// </summary>
Module::Module() {
	// ----- Init ----- //
	id_ = -1;
	ob_vec_v_.resize(ob_end);
	ob_vec_upd_v.resize(ob_end);
	ob_rot_vec_v.resize(ob_end);
	ob_rot_vec_upd_v.resize(ob_end);
}

/// <summary>
/// Update render position / orientation
/// (according to module coordinate)
/// <param name="delta_time">game tick time </param>
/// </summary>
void Module::partsUpdate(float delta_time) {
	for (auto p : parts_) {
		p.mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(p.ofs_pos_, rot_upd_);
		p.mesh_->rot_q_ = p.ofs_rot_ * rot_upd_;
	}
}

/// <summary>
/// show the parts to the camera
/// </summary>
/// <param name="camera"> game camera </param>
void Module::partsRender(dxe::Camera* camera) {
	for (auto p : parts_) {
		if (!p.is_render_) { continue; }
		p.mesh_->render(camera);
	}
}

/// <summary>
/// Perform partsUpdate to all child modules (pre order)
/// </summary>
/// <param name="mod"> module class (parent)</param>
/// <param name="delta_time"></param>
void Module::partsUpdateTree(const Module* mod, float delta_time) {
	partsUpdate(delta_time);
	if (children_.size() == 0) { return; }
	for (auto c : children_) { c->partsUpdateTree(c, delta_time); }
}

/// <summary>
/// Perform partsRender to all child modules (pre order)
/// </summary>
/// <param name="mod"> module cllass (parent)</param>
/// <param name="camera"> game camera </param>
void Module::partsRenderTree(const Module* mod, dxe::Camera* camera) {
	partsRender(camera);
	if (children_.size() == 0) { return; }
	for (auto c : children_) { c->partsRenderTree(c, camera); }
}

/// <summary>
/// return the pointer after creating the module
/// </summary>
/// <param name="id"> module id</param>
/// <param name="name"> module name </param>
/// <param name="pos"> module pos (relative to world :ƒ°0)</param>
/// <param name="ob_vec_v"> x, y, z unit vector in coordinate</param>
/// <param name="ob_rot_vec_v"> need to 3 axis rot vector </param>
/// <param name="ofs_rot"> not required : blank </param>
/// <returns></returns>
Module* Module::createModule(
	int id,
	std::string name,
	tnl::Vector3 pos,
	std::vector<tnl::Vector3> ob_vec_v,
	std::vector<tnl::Vector3> ob_rot_vec_v,
	tnl::Quaternion ofs_rot
	) {
	Module* mod = new Module();
	mod->id_ = id;
	mod->name_ = name;
	mod->ob_vec_v_ = ob_vec_v;
	mod->ob_rot_vec_v = ob_rot_vec_v;
	mod->ofs_rot_ = ofs_rot;

	return mod;
}

/// <summary>
/// attach child module and set up dk_st
/// </summary>
/// <param name="parent"></param>
/// <param name="child"></param>
/// <param name="type"></param>
void Module::attachModule(Module* parent, Module* child, attach_type type) {
	// ---- attach mod. ---- //
	parent->children_.push_back(child);
	child->parent_ = parent;
	// ---- set up dk_st ---- //
	tnl::Vector3 delta_pos;
	float delta_length;
	if (attach_type::absolute == type) { delta_pos = child->pos_ - parent->pos_; }
	else if (attach_type::relative == type) { delta_pos = child->pos_; }
	delta_length = delta_pos.length();
	delta_pos.normalize();		// pos to dir
	parent->setDK_st({
		child->id_,
		child->name_,
		delta_pos,	// dir
		delta_length,
		tnl::Quaternion::RotationAxis({0, 1, 0}, 0),
		true
		});
}


// ----- getter, setter ----- //

void Module::setDK_st(dk_st dk) {
	dk_st_init_.push_back(dk);
}






