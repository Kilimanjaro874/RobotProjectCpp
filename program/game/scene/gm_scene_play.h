#include "gm_scene_base.h"
#include "../model/gm_faceVec.h"
#include "../model/gm_agn_armR001.h"
#include"../model/gm_airplane.h"
#include "../model/gm_parts.h"

#include "../model/gm_Robot.h"



class GmCamera;
class Robot;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	
	Robot* robo_ = nullptr;

	//// test 
	//Parts* parts_ = nullptr;
	//std::vector<dxe::Mesh*> obj_parts_;

	std::vector<FaceVec*> targets_;
	std::vector<FaceVec*> objects_;

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;
};

