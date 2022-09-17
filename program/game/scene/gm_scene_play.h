#include "gm_scene_base.h"

class GmCamera;
//class MdlAirPlane;
class MdlRobotModule;
class MdlRobotAgent_Rarms;

class MdlArm_r;

class Player;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	//MdlAirPlane* plane_ = nullptr;
	// -- ここにロボットクラスを宣言して使用したい -- //
	MdlRobotModule* arm01_ = nullptr;
	MdlRobotAgent_Rarms* armAgnt01_ = nullptr;
	
	MdlArm_r* arm_r_ = nullptr;

	Player* player_ = nullptr;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

