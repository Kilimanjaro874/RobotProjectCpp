#include "gm_scene_base.h"
#include "../model/gm_faceVec.h"
#include "../model/gm_agn_armR001.h"
#include"../model/gm_airplane.h"
#include "../model/gm_parts.h"

class GmCamera;
class Agn_armR001;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	Agn_armR001* armR_ = nullptr;

	std::vector<FaceVec*> targets_;
	std::vector<FaceVec*> objects_;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;
};

