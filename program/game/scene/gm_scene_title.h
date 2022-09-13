#include "gm_scene_base.h"

class SceneTitle : public SceneBase {
public:
	SceneTitle() {}
	~SceneTitle() {}

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};
