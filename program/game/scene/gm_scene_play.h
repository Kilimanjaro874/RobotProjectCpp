#include "gm_scene_base.h"
#include "../model/gm_module.h"
#include "../model/gm_robot.h"
#include "../gm_robot_controller.h"

class GmCamera;

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- ÉÅÉìÉoïœêî ----- //
	GmCamera* _camera = nullptr;
	Robot* _robo = nullptr;
	RobotCont* _controller = nullptr;
	Parts* _background = nullptr;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	// --- objects --- //
	struct Target {
		tnl::Vector3 _pos;
		tnl::Vector3 _rot_axis;
		float _angle;
		tnl::Quaternion _rot;
	};

	Target _targets[10];

};

