#pragma once
#include "gm_robot_define.h"

class Module {
private:
	Module();
	virtual ~Module() {
		for (auto p : parts_) { delete p.mesh_; }
	}

	// ----- General ----- //
	int id_;							// Unique ID for tree structure exp. etc... (error : -1)
	std::string name_;					// Unique name for tree structure exp. etc...
	Module* parent_ = nullptr;			// Store parent (unique)
	std::vector<Module*> children_;		// Store children

	// ----- Render ----- //
	struct parts_st
	{
		bool is_render_ = true;			// draw flag
		tnl::Vector3 ofs_pos_;			// offset distance from module coordinate
		tnl::Quaternion ofs_rot_;		// offset rot with module coordinate
		dxe::Mesh* mesh_ = nullptr;		// mesh object pointer
	};
	std::vector<parts_st> parts_;		// render parts st.
	bool is_render_ = true;				// render : draw flag(batch setting)
	
	// ----- Coordinate system ------ //
	tnl::Vector3 pos_;					// module position (ref: World[ƒ°0])
	std::vector<tnl::Vector3> ob_vec_v_;		// orthonormal basis vectors (x, y, z): init def
	std::vector<tnl::Vector3> ob_vec_upd_v;	// store ob_vec_v_ change value
	std::vector<tnl::Vector3> ob_rot_vec_v;		// axis of rotation (unit vectors)
	std::vector<tnl::Vector3> ob_rot_vec_upd_v;	// store ob_rot_vec_v change value
	enum code {
		ob_x_, ob_y_, ob_z_, ob_end
	};
	tnl::Quaternion ofs_rot_;			// offset rot amount of mod. coordinate
	tnl::Quaternion rot_upd_;			// rot update value
	// ----- Forward Kinematics ----- //
	struct dk_st {
		// ---- FK dataset definition for child Module ---- //
		int child_id_;
		std::string child_name_;
		tnl::Vector3 child_dir_;		// orientation vec from this mod. to child mod.
		float dir_length_;				// length of child_dir_;
		tnl::Quaternion rot_sum_;		// total amound of rotation from parent
		bool is_effect_ik_ = true;		// flag to affecto IK to child
	};
	std::vector<dk_st> dk_st_init_;		// init value;
	std::vector<dk_st> dk_st_up_;		// update value;
	enum attach_type {
		absolute, relative				// Module attachment : absolute / relative ref.
	};
	
	// ----- Inverse Kinematics ----- //
	struct ik_st {
		// ---- IK dataset definition for child Module ---- //
		int child_id_;
		std::string child_name_;
		code rot_vec_;					// specity which rot axis to rotate
		int type_ik_;
		float kp_;						// Coefficient of IK
		Module* target_;				// IK target
		Module* object_;				// IK controll object
	};
	std::vector<ik_st> ik_st_init_;		// init value;
	std::vector<ik_st> ik_st_up_;		// update value;
	
public:
	virtual void partsUpdate(float delta_time);
	virtual void partsRender(dxe::Camera* camera);
	void partsUpdateTree(const Module* mod, float delta_time);
	void partsRenderTree(const Module* mod, dxe::Camera* camera);
	static Module* createModule(
		int id,
		std::string name,
		tnl::Vector3 pos,
		std::vector<tnl::Vector3> ob_vec_v,
		std::vector<tnl::Vector3> ob_rot_vec_v,
		tnl::Quaternion ofs_rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)	 // no-rotation q
	);
	void attachModule(Module* parent, Module* child, attach_type type = relative);
	

	// ----- getter, setter ----- //
	void setDK_st(dk_st dk);
};