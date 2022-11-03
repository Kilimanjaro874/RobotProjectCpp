#include "gm_scene_base.h"
#include "../model/gm_module.h"
#include "../model/gm_robot.h"
#include "../gm_robot_controller.h"

class GmCamera;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* _camera = nullptr;
	Robot* _robo = nullptr;
	RobotCont* _controller = nullptr;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

