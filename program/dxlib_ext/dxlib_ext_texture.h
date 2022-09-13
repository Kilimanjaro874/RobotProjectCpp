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

		// �t�@�C���p�X�̎擾
		const std::string& getFilePath() { return file_path_; }
		// �O���t�B�b�N�n���h���̎擾
		int getGraphHandle() { return graph_hdl_; }

		// ����
		// arg1... �t�@�C���p�X
		static std::shared_ptr<Texture> CreateFromFile( const std::string& file_path );

	};

}

