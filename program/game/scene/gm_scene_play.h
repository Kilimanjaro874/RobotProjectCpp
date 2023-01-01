#include "gm_scene_base.h"
#include "../model/gm_coordinate.h"
#include "../model/gm_module.h"
#include "../model/gm_coordinate_mgr.h"

class GmCamera;
class AnimSprite3D;
class ScenePlay : public SceneBase {
public :
	ScenePlay() {};
	~ScenePlay();

	GmCamera* camera_ = nullptr;

	CoordinateMgr* co_mgr_;

	std::vector<Module* > mod_;
	std::vector<Coordinate* > coord_;
	
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

