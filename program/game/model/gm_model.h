#pragma once
#include "gm_parts.h"


class Model {
public :
	Model(){};
	virtual ~Model() {
		for (auto pts : parts_) delete pts;
	}
	tnl::Vector3 pos_;
	tnl::Quaternion rot_;
	std::vector<Parts*> parts_;

	//----------------------------------------------------------------------------------------------------------
	// バイナリ形式保存
	// arg1... 保存先ファイルパス
	// tips... 複数のパーツで構成したモデルデータとしてバイナリ形式で保存する
	// tips... この関数で保存するには各パーツのメッシュを プログラマブル形式で生成している事
	void exportForFileBinary( const std::string& file_path );

	//----------------------------------------------------------------------------------------------------------
	// バイナリ形式保存されたモデルデータの読み込み
	// arg1... ファイルパス
	// tips... exportForFileBinary で保存されたファイルの読み込み
	void loadFromFileBinary( const std::string& file_path );


	virtual void update(float delta_time);


	virtual void render(dxe::Camera* camera);

};



