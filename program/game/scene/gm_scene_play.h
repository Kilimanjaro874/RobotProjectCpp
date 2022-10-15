#include "gm_scene_base.h"
#include "../model/gm_module.h"
#include "../model/gm_face_vector.h"
#include "../model/gm_robot.h"

class GmCamera;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	FaceVector* target_ = nullptr;
	Robot* robo_ = nullptr;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

