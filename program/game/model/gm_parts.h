#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Parts {
public :
	Parts(){}
	~Parts() {
		delete mesh_;
	}
	bool is_render_ = true;
	tnl::Vector3 ofs_pos_;
	tnl::Quaternion ofs_rot_;
	dxe::Mesh* mesh_ = nullptr ;
};


