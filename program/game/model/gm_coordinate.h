#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "gm_parts.h"

class Coordinate {
public:
	Coordinate() {};
	virtual ~Coordinate() {
		for (auto cod : coordinate_parts_) delete cod;
	};

	enum class attach_type {
		absolute, relative
	};
	enum class ik_type {
		// object - target //
		pos_to_pos,
		dirx_as_dirx, dirx_as_diry,	dirx_as_dirz,
		diry_as_diry, diry_as_dirx,	diry_as_dirz,
		dirz_as_dirz, dirz_as_dirx,	dirz_as_diry,
		dirx_look_pos,
		diry_look_pos,
		dirz_look_pos,
		as_it_was,
	};

	enum class const_condition {
		// object - target //
		rot_as_rot,
	};

	struct dk_st_delta_ {
		bool is_update = false;
		tnl::Vector3 dir_;
		float length_;
	};
	dk_st_delta_ dk_st_upd_delta_;

protected:
	// ----- General ----- //
	int id_;
	std::string name_;
	Coordinate* parent_;
	struct tree_st_ {
		int com_type;
		int com_col;
		int com_row;
	};
	tree_st_ tree_st_data_;
	enum class coordinate {
		x, y, z, end
	};
	tnl::Vector3 pos_;
	std::vector<tnl::Vector3> 
		oc_vec_v_, 
		oc_vec_upd_v_,
		oc_rot_vec_v_,
		oc_rot_vec_upd_v_;
	tnl::Quaternion
		oc_rot_,
		oc_rot_upd_;
	// ----- Render ----- //
	std::vector<Parts*> coordinate_parts_;

	// ----- Forward Kinematics ----- //
	tnl::Vector3 pos_from_parent_;
	tnl::Quaternion rot_from_parent_;
	struct dk_st_ {
		Coordinate* coord_;
		tnl::Vector3 dir_;
		float length_;
	};
	std::vector<dk_st_> children_;

	// ----- Inverse Kinematics ----- //
	struct ik_st_ {
		Coordinate* object_;
		Coordinate* target_;
		ik_type ik_type_;
		float kp_;
		bool is_rot_[static_cast<int>(coordinate::end)] = { true, true, true };
	};
	std::vector<ik_st_> ik_settings_;

public:
	virtual void init();
	virtual void update(float delta_time);
	virtual void render(dxe::Camera* camera);
	void setCoordinate(
		int id,
		std::string name,
		tnl::Vector3 pos = {0, 0, 0},
		tnl::Vector3 x = {1, 0, 0}, 
		tnl::Vector3 y = {0, 1, 0},
		tnl::Vector3 z = {0, 0, 1},
		tnl::Vector3 rot_x = {1, 0, 0},
		tnl::Vector3 rot_y = {0, 1, 0},
		tnl::Vector3 rot_z = {0, 0, 1},
		tnl::Quaternion offset_rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0)
	);
	
	void setChildAndDKInit(Coordinate* child, attach_type type);
	void directKinematics(tnl::Quaternion ik_rot, float delta_time, bool is_do_ik = true);
	void setIKObjectTargetInit(Coordinate* object, Coordinate* target, ik_type type, float kp,
		bool is_rot_axis_x = true, bool is_rot_axis_y = true, bool is_rot_axis_z = true);
	tnl::Quaternion inverseKinematics(float delta_time);
	void constraintAdd(Coordinate* target, const_condition constraint);
	// ----- setter, getter ----- //
	void setParentParams(tnl::Vector3 pos,tnl::Quaternion rot) {
		pos_from_parent_ = pos;  
		rot_from_parent_ = rot;
	}
	void setIK_st(const ik_st_* ik) { ik_settings_.push_back(*ik); }
	void setTranslate(tnl::Vector3 move, attach_type type = attach_type::relative);
	int getId() { return id_; }
	std::string getName() { return name_; }
	void setViewCoorinate(float length, float radius);
	void setParent(Coordinate* parent) { parent_ = parent; }
	tnl::Vector3 getPos() { return pos_; }
	tnl::Quaternion getRot() { return oc_rot_upd_; }
	tree_st_ getTreeLocateInfo() { return tree_st_data_; }
	tnl::Vector3 getDirX() { return oc_vec_upd_v_[static_cast<int>(coordinate::x)]; }
	tnl::Vector3 getDirY() { return oc_vec_upd_v_[static_cast<int>(coordinate::y)]; }
	tnl::Vector3 getDirZ() { return oc_vec_upd_v_[static_cast<int>(coordinate::z)]; }
	tnl::Vector3 getRotX() { return oc_rot_vec_upd_v_[static_cast<int>(coordinate::x)]; }
	tnl::Vector3 getRotY() { return oc_rot_vec_upd_v_[static_cast<int>(coordinate::y)]; }
	tnl::Vector3 getRotZ() { return oc_rot_vec_upd_v_[static_cast<int>(coordinate::z)]; }
};