#include "gm_scene_base.h"

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- �����o�ϐ� ----- //
	GmCamera* _camera = nullptr;

	//// ----- �����o�֐� ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

