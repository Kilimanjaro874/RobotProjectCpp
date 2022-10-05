#include "gm_scene_base.h"
#include "../model/gm_faceVec.h"
#include "../model/gm_agn_armR001.h"

class GmCamera;
class FaceVec;
class Agn_armR001;

class ScenePlay : public SceneBase {
public :
	ScenePlay(){}
	~ScenePlay();

	GmCamera* camera_ = nullptr;
	// --- 目標位置・姿勢のリスト作成 --- //
	FaceVec* aim_tar_ = nullptr;
	FaceVec* elbow_r_tar = nullptr;
	std::vector<FaceVec*> targets_;
	// --- 制御位置・姿勢のリスト作成 --- //
	FaceVec* effector_obj_ = nullptr;
	FaceVec* elbow_obj_ = nullptr;
	std::vector<FaceVec*> objects_;

	Agn_armR001* arm_r = nullptr;



	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

};

