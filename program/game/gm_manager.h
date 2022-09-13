#pragma once
#include "../dxlib_ext/dxlib_ext.h"

class SceneBase;
class GameManager {
private :
	GameManager();
	~GameManager();

	int img_black_ = 0;

	static GameManager* instance_;

	// 画面遷移演出の時間
	const float SCENE_TRANSITION_FIX_TIME = 0.5f;
	float scene_transition_time_ = 0;
	float scene_transition_alpha_ = 1.0f ;

	tnl::Sequence<GameManager> sequence_ = 
		tnl::Sequence<GameManager>(this, &GameManager::seqSceneInTransition);

	// 画面遷移( フェードインプロセス )
	bool seqSceneInTransition(const float delta_time);

	// ゲームプロセス
	bool seqGameProcess( const float delta_time );

	// 画面遷移( フェードアウトプロセス )
	bool seqSceneOutTransition(const float delta_time);

	// 現在のシーン
	SceneBase* scene_now_ = nullptr;
	// 予約シーン
	SceneBase* scene_next_ = nullptr;

public :

	// インスタンスの取得
	static GameManager* GetInstance();

	// 破棄
	static void Destroy();

	// 更新
	void update(float delta_time);

	// シーンの変更
	// arg1... SceneBase を継承したクラスインスタンス
	void chengeScene(SceneBase* next);

public :

	// ゲーム全体で参照したい変数はここで用意


};




