#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <memory>
#include "DxLib.h"
namespace dxe {

	class Texture final {
	private :
		Texture(){}
		std::string file_path_;
		int graph_hdl_ = 0;
	public :
		~Texture() {
			DeleteGraph(graph_hdl_);
		}

		// ファイルパスの取得
		const std::string& getFilePath() { return file_path_; }
		// グラフィックハンドルの取得
		int getGraphHandle() { return graph_hdl_; }

		// 生成
		// arg1... ファイルパス
		static std::shared_ptr<Texture> CreateFromFile( const std::string& file_path );

	};

}

