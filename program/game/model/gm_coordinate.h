#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Coordinate {
/// <summary>
/// 
/// </summary>
public:
	Coordinate() {}
	virtual ~Coordinate() {

	}

private:
	enum class coordinate {
		x, y, z, end
	};

};