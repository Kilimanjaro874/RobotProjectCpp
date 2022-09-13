#include "gm_scene_base.h"

class GmCamera;
class MdlAirPlane;
class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	MdlAirPlane* plane_ = nullptr;
	// -- ここにロボットクラスを宣言して使用したい -- //



	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

