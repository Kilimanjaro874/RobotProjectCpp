#include "gm_scene_base.h"
#include "../model/gm_model.h"
#include "../model/gm_agent.h"
#include "../model/gm_robot.h"

class GmCamera;
//class MdlAirPlane;

class MdlRbArmR;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	//MdlAirPlane* plane_ = nullptr;
	// -- ここにロボットクラスを宣言して使用したい -- //

	
	MdlRbArmR* arm_r_ = nullptr;

	Parts* target_View_ = nullptr;
	Parts* elbowTar_View_ = nullptr;
	//Robot* robot_ = nullptr;

	tnl::Vector3 origin_ = { 0, 0, 0 };

	tnl::Vector3 pos_ = { 30, 50, 0 };
	tnl::Vector3 pos_dir_ = tnl::Vector3::Normalize(pos_);
	float pos_length = pos_.length();

	tnl::Vector3 forward_ = { 0, 0, 1 };
	tnl::Vector3 rotAi_ = { 0, 1, 0 };
	tnl::Quaternion rot_ = tnl::Quaternion::RotationAxis(rotAi_, 0);
	tnl::Quaternion rot_sum_ = tnl::Quaternion::RotationAxis(rotAi_, 0);
	tnl::Vector3 targetPos_ = { 0, 0, 200 };	// 手先目標位置初期値

	tnl::Vector3 elbowRefPos_ = { 50, 0, 0 };	// 肘目標位置初期値
	tnl::Vector3 el_dir_ = tnl::Vector3::Normalize(elbowRefPos_);
	float el_length = elbowRefPos_.length();

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

