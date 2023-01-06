#include "gm_robot_mgr.h"

void RobotMgr::init() {
	CoordinateMgr::init();
	getCoordinateDataCSV("CoordinateData_Robot1_01.csv");
}

