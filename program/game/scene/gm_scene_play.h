#include "gm_scene_base.h"
#include "../model/gm_module.h"
#include "../model/gm_robot.h"
#include "../gm_robot_controller.h"
#include "../model/gm_shot_taget.h"
#include "../gm_hit_manager.h"

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- メンバ変数 ----- //
	GmCamera* _camera = nullptr;
	Robot* _robo = nullptr;
	RobotCont* _controller = nullptr;
	Parts* _background = nullptr;
	Parts* _aim_pos = nullptr;
	HitManager* _hit_manager;

	// --- Map object --- //
	std::vector<Parts*> _field_obj_li;	// 
	std::vector<dxe::Mesh*> _floor_obj_li;	//	床メッシュ取得

	// --- ShotTargetObject --- //
	std::vector<ShotTarget*> _target_obj_v;

	// --- UI --- //
	int _sight_UI_gh;						// ロボットのサイト表示
	
	// --- flag --- //
	bool _is_clear = false;
	float _clear_time = 0;

	//// ----- メンバ関数 ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;
	void setTargets();
};

