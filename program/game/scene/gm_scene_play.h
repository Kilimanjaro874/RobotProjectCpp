#include "gm_scene_base.h"

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- メンバ変数 ----- //
	GmCamera* _camera = nullptr;

	//// ----- メンバ関数 ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

