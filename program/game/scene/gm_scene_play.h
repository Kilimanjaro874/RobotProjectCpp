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

	tnl::Vector3 pos_ = { 0, 0, 0 };
	tnl::Vector3 forward_ = { 1, 0, 0 };
	tnl::Vector3 rotAi_ = { 0, 1, 0 };
	tnl::Quaternion tempQ_;

	tnl::Vector3 targetPos_ = { 100, 0, 0 };	// 手先目標位置初期値

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

