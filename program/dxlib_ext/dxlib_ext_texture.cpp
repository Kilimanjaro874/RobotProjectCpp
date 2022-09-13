#include "dxlib_ext_texture.h"

namespace dxe {

	std::shared_ptr<Texture> Texture::CreateFromFile(const std::string& file_path)
	{
		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>( new Texture() );
		tex->file_path_ = file_path;
		tex->graph_hdl_ = LoadGraph(file_path.c_str()) ;
		return tex;
	}

}

