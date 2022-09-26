#include "gm_scene_base.h"
#include "../model/gm_faceVec.h"

class GmCamera;
class FaceVec;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;

	FaceVec* cont_tar01_ = nullptr;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

