#include "gm_scene_base.h"

class SceneResult : public SceneBase {
public:
	SceneResult() {}
	~SceneResult() {}

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};
