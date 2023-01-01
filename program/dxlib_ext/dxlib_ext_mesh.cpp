#include "dxlib_ext_camera.h"
#include "dxlib_ext_mesh.h"
#include "dxlib_ext_data.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	void Mesh::createPlaneIndex( const int div_w, const int div_h, const bool is_left_cycle) {
		int index_num = div_w * div_h * 6;
		idxs_.resize(index_num);

		// 頂点インデックスの計算
		for (int i = 0; i < div_h; ++i) {
			for (int k = 0; k < div_w; ++k) {
				int a = (i * div_w + k) * 6;

				// 左回りインデックス
				if(is_left_cycle) {

					// 左上の三角形
					idxs_[a + 2] = (i * (div_w + 1)) + k;
					idxs_[a + 1] = idxs_[a + 2] + 1;
					idxs_[a + 0] = idxs_[a + 2] + (div_w + 1);

					// 右下の三角形
					idxs_[a + 5] = idxs_[a + 0];
					idxs_[a + 4] = idxs_[a + 1];
					idxs_[a + 3] = idxs_[a + 5] + 1;
				}
				// 右回りインデックス
				else{
					// 左上の三角形
					idxs_[a + 0] = (i * (div_w + 1)) + k;
					idxs_[a + 1] = idxs_[a + 0] + 1;
					idxs_[a + 2] = idxs_[a + 0] + (div_w + 1);
					// 右下の三角形
					idxs_[a + 3] = idxs_[a + 2];
					idxs_[a + 4] = idxs_[a + 1];
					idxs_[a + 5] = idxs_[a + 3] + 1;
				}

			}
		}
	}

	//----------------------------------------------------------------------------------------
	void Mesh::createVBO() {
		vb_hdl_ = CreateVertexBuffer((int)vtxs_.size(), DX_VERTEX_TYPE_NORMAL_3D);
		ib_hdl_ = CreateIndexBuffer((int)idxs_.size(), DX_INDEX_TYPE_32BIT);
		SetVertexBufferData(0, vtxs_.data(), (int)vtxs_.size(), vb_hdl_);
		SetIndexBufferData(0, idxs_.data(), (int)idxs_.size(), ib_hdl_);
	}

	//----------------------------------------------------------------------------------------
	bool Mesh::exportForFileFormatX(const std::string& file_path) {
		std::string str = "\n Mesh{ \n";
		str += std::to_string( vtxs_.size() ) + "; \n" ;
		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			str += std::to_string(vtxs_[i].pos.x) + "; ";
			str += std::to_string(vtxs_[i].pos.y) + "; ";
			str += std::to_string(vtxs_[i].pos.z) + ";";
			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += std::to_string(idxs_.size()/3) + "; \n";
		for (uint32_t i = 0; i < idxs_.size(); i += 3) {
			str += "3;";
			str += std::to_string(idxs_[i + 0]) + ",";
			str += std::to_string(idxs_[i + 1]) + ",";
			str += std::to_string(idxs_[i + 2]) + ";";
			str += ((idxs_.size() - 3) != i) ? ",\n" : ";\n";
		}
		str += "MeshMaterialList{\n";
		str += "1;\n";
		str += std::to_string(idxs_.size() / 3) + ";\n";
		for (uint32_t i = 0; i < idxs_.size() / 3; ++i) {
			str += "0";
			str += (((idxs_.size() / 3) - 1) != i) ? ",\n" : ";;\n";
		}
		str += "Material { \n";

		// diff
		//str += "1.000000; 1.000000; 1.000000; 0.000000;;\n";
		str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.r) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.g) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.b) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.a) + ";;\n";

		// power
		//str += "5.000000;\n";
		str += std::to_string(render_param_.dxlib_mtrl_.Power) + ";\n";

		// spec
		//str += "0.250000; 0.250000; 0.250000;;\n";
		str += std::to_string(render_param_.dxlib_mtrl_.Specular.r) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Specular.g) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Specular.b) + ";;\n";

		// emissive
		//str += "0.000000; 0.000000; 0.000000;;\n";
		str += std::to_string(render_param_.dxlib_mtrl_.Emissive.r) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Emissive.g) + ";";
		str += std::to_string(render_param_.dxlib_mtrl_.Emissive.b) + ";;\n";

		str += "TextureFilename{\n";
		const char t = '"';
		str += t;
		str += "test.bmp";
		str += t;
		str += ";\n";
		str += "}\n}\n}\n";


		//----------------------------------------------------------------------------------------------------------

		str += "MeshNormals{\n";
		str += std::to_string(vtxs_.size()) + "; \n";

		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			str += std::to_string(vtxs_[i].norm.x) + "; ";
			str += std::to_string(vtxs_[i].norm.y) + "; ";
			str += std::to_string(vtxs_[i].norm.z) + ";";
			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += std::to_string(idxs_.size() / 3) + "; \n";
		for (uint32_t i = 0; i < idxs_.size(); i += 3) {
			str += "3;";
			str += std::to_string(idxs_[i + 0]) + ",";
			str += std::to_string(idxs_[i + 1]) + ",";
			str += std::to_string(idxs_[i + 2]) + ";";
			str += ((idxs_.size() - 3) != i) ? ",\n" : ";\n";
		}
		str += "}\n";

		//----------------------------------------------------------------------------------------------------------



		str += "MeshTextureCoords{\n";
		str += std::to_string(vtxs_.size()) + "; \n";
		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			str += std::to_string(vtxs_[i].u) + "; ";
			str += std::to_string(vtxs_[i].v) + "; ";
			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += "}\n";
		str += "MeshVertexColors {\n";
		str += std::to_string(vtxs_.size()) + "; \n";
		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			str += std::to_string(i) + "; ";
			str += "1.000000;1.000000;1.000000;1.000000;";
			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += "}\n}\n";

		std::string save_str = g_x_file_hedder_str + str ;

		FILE* fp = nullptr;
		if (fopen_s(&fp, file_path.c_str(), "w") == 0) {
			fwrite(save_str.data(), save_str.length(), 1, fp);
			fclose(fp);
		}
		return true;
	}


	//----------------------------------------------------------------------------------------
	bool Mesh::exportForFileFormatMV(const std::string& file_path) {
		if (mesh_format_ != eMeshFormat::MESH_FMT_PG) return false;
		Mesh* mesh;
		exportForFileFormatX("temp.x");
		mesh = CreateFromFileMV("temp.x");
		DeleteFileA("temp.x");		
		MV1SaveModelToMV1File(mesh->mv_hdl_, file_path.c_str());
		delete mesh;
		return true;
	}


	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateFromFileMV(const std::string& file_path, const float scl)
	{
		Mesh* mesh = new Mesh();
		mesh->mesh_format_ = Mesh::eMeshFormat::MESH_FMT_MV;
		mesh->mv_hdl_ = MV1LoadModel(file_path.c_str());
		mesh->scl_ = { scl, scl, scl };
		DxLib::VECTOR dxv = MV1GetMeshMaxPosition(mesh->mv_hdl_, 0);
		mesh->bd_box_size_ = tnl::Vector3(dxv.x, dxv.y, dxv.z) * 2.0f ;
		mesh->bd_sphere_radius_ = mesh->bd_box_size_.length() * 0.5f ;

		return mesh;
	}


	//----------------------------------------------------------------------------------------
	void Mesh::render(const Camera* camera) {

		MATRIX view, proj;
		memcpy(view.m, camera->view_.m, sizeof(float) * 16);
		memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
		SetCameraViewMatrix(view);
		SetupCamera_ProjectionMatrix(proj);

		SetLightEnable(render_param_.is_default_light_enable_);

		// 頂点の射影変換用行列
		tnl::Matrix mt_vertex;

		// 頂点のワールド行列
		tnl::Matrix mt_vtx_world;

		// オブジェクトとしての座標・回転・スケール・ワールド行列
		tnl::Matrix mt_trans;
		tnl::Matrix mt_rotation;
		tnl::Matrix mt_scale;
		tnl::Matrix mt_obj_world;

		// オブジェクトのワールド行列の作成
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);

		mt_rotation = rot_q_.getMatrix();

		mt_scale = tnl::Matrix::Scaling(scl_.x, scl_.y, scl_.z);

		// ワールド行列を計算 ( スケール行列 x 回転行列 x 座標行列 )
		// ※ 右手座標系は行列の掛け順が逆になるので注意
		mt_obj_world = mt_scale * mt_rotation * mt_trans;

		MATRIX dxm;
		memcpy(dxm.m, mt_obj_world.m, sizeof(float)*16);
		SetMaterialParam(render_param_.dxlib_mtrl_);

		int i_alpha = (int)(render_param_.alpha_ * 255.0f );
		if (Mesh::eMeshFormat::MESH_FMT_PG == mesh_format_) {
			SetTransformToWorld(&dxm);
			SetUseZBuffer3D(render_param_.is_depth_test_enable_);
			SetWriteZBuffer3D(render_param_.is_write_depth_buffer_enable_);
			SetUseBackCulling(render_param_.dxlib_culling_mode_);
			SetDrawBlendMode(render_param_.dxlib_blend_mode_, i_alpha);
			//DrawPrimitiveIndexed3D(vtxs_.data(), vtxs_.size(), idxs_.data(), idxs_.size(), DX_PRIMTYPE_TRIANGLELIST, tex_diffuse_hdl_, false);
			DrawPrimitiveIndexed3D_UseVertexBuffer(vb_hdl_, ib_hdl_, DX_PRIMTYPE_TRIANGLELIST, tex_diffuse_->getGraphHandle(), true);
		}
		else {
			MATRIX dxim;
			CreateIdentityMatrix(&dxim);
			SetTransformToWorld(&dxim);
			MV1SetUseZBuffer(mv_hdl_, render_param_.is_depth_test_enable_);
			MV1SetWriteZBuffer(mv_hdl_, render_param_.is_write_depth_buffer_enable_);
			MV1SetMeshBackCulling(mv_hdl_, 0, render_param_.dxlib_culling_mode_);
			MV1SetSampleFilterMode(mv_hdl_, render_param_.dxlib_sample_filter_mode_);
			MV1SetMeshDrawBlendMode(mv_hdl_, 0, render_param_.dxlib_blend_mode_);
			MV1SetMeshDrawBlendParam(mv_hdl_, 0, i_alpha);
			MV1SetMatrix(mv_hdl_ , dxm);
			MV1DrawModel(mv_hdl_);
		}

		//---------------------------------------------------------------------
		//
		// デバッグライン
		//
		SetWriteZBuffer3D(FALSE);
		SetUseZBuffer3D(FALSE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AXIS)			DrawAxis(camera, pos_, rot_q_, bd_sphere_radius_);
		SetUseZBuffer3D(TRUE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_OBB)			DrawOBB(camera, pos_, rot_q_, bd_box_size_);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_BD_SPHERE)	DrawBdSphere(camera, pos_, bd_sphere_radius_);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_VTX_NML)		drawVtxNormal();
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AABB)			DrawAABB(camera, pos_, bd_box_size_);
	}


	void Mesh::drawVtxNormal() {
		float length = 3.0f;
		for (auto &v : vtxs_) {
			DxLib::VECTOR nv;
			nv.x = v.pos.x + v.norm.x * length;
			nv.y = v.pos.y + v.norm.y * length;
			nv.z = v.pos.z + v.norm.z * length;
			DrawLine3D(v.pos, nv, 0xffffff00);
		}
	}


	Mesh* Mesh::CreateConvertMV(Mesh* mesh) {
		Mesh* new_mesh;
		mesh->exportForFileFormatX("temp.x");
		delete mesh;
		new_mesh = CreateFromFileMV("temp.x");
		DeleteFileA("temp.x");
		return new_mesh;
	}


}
