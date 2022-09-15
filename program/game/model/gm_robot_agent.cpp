#include "gm_robot_agent.h"

MdlRobotAgent_Rarms* MdlRobotAgent_Rarms::Create() {
	
	MdlRobotAgent_Rarms* agn = new MdlRobotAgent_Rarms();
	agn->modules_.resize(e_models_max);

	MdlRobotModule* shoulder = new MdlRobotModule();


}