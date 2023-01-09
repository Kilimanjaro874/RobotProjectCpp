#include "gm_scene_base.h"

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();
	//// ----- ƒƒ“ƒo•Ï” ----- //
	GmCamera* _camera = nullptr;

	//// ----- ƒƒ“ƒoŠÖ” ----- ////
	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

