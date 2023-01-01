#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Coordinate {
public:
	Coordinate() {};
	virtual ~Coordinate() {};

	enum attach_type {
		absolute, relative
	};

private:
	// ----- General ----- //
	int id_;
	std::string name_;
	struct tree_st_ {
		int com_type;
		int com_col;
		int com_row;
	};
	tree_st_ tree_st_data_;
	enum coordinate {
		co_x, co_y, co_z, co_end
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

	// ----- Tree Structure ----- //
	// ----- Forward Kinematics ----- //
	tnl::Vector3 pos_from_parent_;
	tnl::Quaternion rot_from_parent_;
	struct dk_st_ {
		Coordinate* coord_;
		tnl::Vector3 dir_;
		float length_;
	};
	std::vector<dk_st_> children_;

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
	void directKinematics(tnl::Quaternion ik_rot);
	// ----- setter, getter ----- //
	void setParentParams(tnl::Vector3 pos,tnl::Quaternion rot) {
		pos_from_parent_ = pos;  
		rot_from_parent_ = rot;
	}
	int getId() { return id_; }
	std::string getName() { return name_; }
	void setTreeLocateInfo(int type, int col, int row);
	tnl::Vector3 getPos() { return pos_; }
	tnl::Quaternion getRot() { return oc_rot_upd_; }
	tree_st_ getTreeLocateInfo() { return tree_st_data_; }
};