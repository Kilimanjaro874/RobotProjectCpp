#include "gm_scene_base.h"

class GmCamera;
class MdlAirPlane;
class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	MdlAirPlane* plane_ = nullptr;
	// -- �����Ƀ��{�b�g�N���X��錾���Ďg�p������ -- //



	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

