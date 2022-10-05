#include "../OBJLoader.h"
#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	std::vector<Mesh*> Mesh::CreateFromFileObj(const std::string& file_path, const float scl)
	{
		OBJMESH* obj = new OBJMESH();
		std::vector<Mesh*> create_meshs;

		obj->LoadFile(file_path.c_str());

		std::tuple<std::string, std::string, std::string> det_paths;
		det_paths = tnl::DetachmentFilePath(file_path);

		for (unsigned int i = 0; i < obj->GetNumSubsets(); i++) {
			OBJMATERIAL* materials = &obj->GetMaterials()[obj->GetSubsets()[i].materialIndex];
			unsigned int* indexs = &obj->GetIndices()[obj->GetSubsets()[i].faceStart];
			OBJVERTEX* vertexs = obj->GetVertices();

			unsigned int vertex_num = obj->GetSubsets()[i].faceCount;
			unsigned int index_num = vertex_num;
			unsigned int triangle_num = vertex_num / 3;

			if (0 == vertex_num) continue;

			Mesh* mesh = new Mesh();
			mesh->vtxs_.resize(vertex_num);
			mesh->idxs_.resize(index_num);

			OBJMATERIAL* mtl = &obj->GetMaterials()[obj->GetSubsets()[i].materialIndex];

			mesh->render_param_.dxlib_mtrl_.Ambient = { mtl->ambient.x, mtl->ambient.y, mtl->ambient.z };
			mesh->render_param_.dxlib_mtrl_.Diffuse = { mtl->diffuse.x, mtl->diffuse.y, mtl->diffuse.z };
			mesh->render_param_.dxlib_mtrl_.Specular = { mtl->specular.x, mtl->specular.y, mtl->specular.z };

			if (!std::string(mtl->ambientMapName).empty()) {
				mesh->tex_ambient_ = dxe::Texture::CreateFromFile((std::get<0>(det_paths) + "/" + std::string(mtl->ambientMapName)).c_str());
				mesh->tex_diffuse_ = mesh->tex_ambient_;

			}
			if (!std::string(mtl->specularMapName).empty()) {
				mesh->tex_specular_ = dxe::Texture::CreateFromFile((std::get<0>(det_paths) + "/" + std::string(mtl->specularMapName)).c_str());
				mesh->tex_diffuse_ = mesh->tex_specular_;
			}
			if (!std::string(mtl->bumpMapName).empty()) {
				mesh->tex_bump_ = dxe::Texture::CreateFromFile((std::get<0>(det_paths) + "/" + std::string(mtl->bumpMapName)).c_str());
				mesh->tex_diffuse_ = mesh->tex_bump_;
			}
			if (!std::string(mtl->diffuseMapName).empty()) {
				mesh->tex_diffuse_ = dxe::Texture::CreateFromFile((std::get<0>(det_paths) + "/" + std::string(mtl->diffuseMapName)).c_str());
			}
			if (!mesh->tex_diffuse_) {
				mesh->tex_diffuse_ = dxe::Texture::CreateFromFile("graphics/white.bmp");
			}

			int tw, th;
			int bw, bh;
			GetGraphSize(mesh->getTexture()->getGraphHandle(), &tw, &th);
			tnl::MostBit32(tw, bw);
			tnl::MostBit32(th, bh);
			int tw2 = 1 << (bw + 1);
			int th2 = 1 << (bh + 1);

			float ru = (float)tw / (float)tw2;
			float rv = (float)th / (float)th2;

			for (unsigned int k = 0; k < triangle_num; ++k) {
				for (unsigned int n = 0; n < 3; ++n) {
					int a = (k * 3) + n;
					float px = vertexs[indexs[a]].position.x * scl;
					float py = vertexs[indexs[a]].position.y * scl;
					float pz = vertexs[indexs[a]].position.z * scl;
					float nx = vertexs[indexs[a]].normal.x;
					float ny = vertexs[indexs[a]].normal.y;
					float nz = vertexs[indexs[a]].normal.z;
					float u = vertexs[indexs[a]].texcoord.x;
					float v = 1.0f - vertexs[indexs[a]].texcoord.y;
					mesh->idxs_[a] = a;
					mesh->vtxs_[a].pos = { px, py, pz };
					mesh->vtxs_[a].norm = { nx, ny, nz };
					mesh->vtxs_[a].u = u * ru;
					mesh->vtxs_[a].v = v * rv;
					mesh->vtxs_[a].dif = GetColorU8(255, 255, 255, 255);
					mesh->vtxs_[a].spc = GetColorU8(255, 255, 255, 255);
				}
			}

			mesh->bd_sphere_radius_ = obj->GetSphere().radius * scl;
			mesh->bd_box_size_.x = (obj->GetBox().maximum.x - obj->GetBox().minimum.x) * scl;
			mesh->bd_box_size_.y = (obj->GetBox().maximum.y - obj->GetBox().minimum.y) * scl;
			mesh->bd_box_size_.z = (obj->GetBox().maximum.z - obj->GetBox().minimum.z) * scl;

			mesh->createVBO();
			create_meshs.emplace_back(mesh);
		}

		delete obj;

		return create_meshs;
	}

}
