#include "gm_scene_base.h"

class GmCamera;
//class MdlAirPlane;
class MdlRobotModule;
class MdlRobotAgent_Rarms;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	//MdlAirPlane* plane_ = nullptr;
	// -- �����Ƀ��{�b�g�N���X��錾���Ďg�p������ -- //
	MdlRobotModule* arm01_ = nullptr;
	MdlRobotAgent_Rarms* armAgnt01_ = nullptr;
	

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

