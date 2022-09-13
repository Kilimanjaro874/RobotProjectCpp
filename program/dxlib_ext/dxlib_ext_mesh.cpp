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
		vb_hdl_ = CreateVertexBuffer(vtxs_.size(), DX_VERTEX_TYPE_NORMAL_3D);
		ib_hdl_ = CreateIndexBuffer(idxs_.size(), DX_INDEX_TYPE_32BIT);
		SetVertexBufferData(0, vtxs_.data(), vtxs_.size(), vb_hdl_);
		SetIndexBufferData(0, idxs_.data(), idxs_.size(), ib_hdl_);
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
		str += std::to_string(mtrl_.Diffuse.r) + ";";
		str += std::to_string(mtrl_.Diffuse.g) + ";";
		str += std::to_string(mtrl_.Diffuse.b) + ";";
		str += std::to_string(mtrl_.Diffuse.a) + ";;\n";

		// power
		//str += "5.000000;\n";
		str += std::to_string(mtrl_.Power) + ";\n";

		// spec
		//str += "0.250000; 0.250000; 0.250000;;\n";
		str += std::to_string(mtrl_.Specular.r) + ";";
		str += std::to_string(mtrl_.Specular.g) + ";";
		str += std::to_string(mtrl_.Specular.b) + ";;\n";

		// emissive
		//str += "0.000000; 0.000000; 0.000000;;\n";
		str += std::to_string(mtrl_.Emissive.r) + ";";
		str += std::to_string(mtrl_.Emissive.g) + ";";
		str += std::to_string(mtrl_.Emissive.b) + ";;\n";

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

		SetLightEnable(is_default_light_enable_);

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
		SetMaterialParam(mtrl_);

		int i_alpha = (int)( alpha_ * 255.0f );
		if (Mesh::eMeshFormat::MESH_FMT_PG == mesh_format_) {
			SetTransformToWorld(&dxm);
			SetUseZBuffer3D(is_depth_test_enable_);
			SetWriteZBuffer3D(is_write_depth_buffer_enable_);
			SetUseBackCulling(dxlib_culling_mode_);
			SetDrawBlendMode(dxlib_blend_mode_, i_alpha);	
			//DrawPrimitiveIndexed3D(vtxs_.data(), vtxs_.size(), idxs_.data(), idxs_.size(), DX_PRIMTYPE_TRIANGLELIST, tex_diffuse_hdl_, false);
			DrawPrimitiveIndexed3D_UseVertexBuffer(vb_hdl_, ib_hdl_, DX_PRIMTYPE_TRIANGLELIST, tex_diffuse_->getGraphHandle(), true);
		}
		else {
			MV1SetUseZBuffer(mv_hdl_, is_depth_test_enable_);
			MV1SetWriteZBuffer(mv_hdl_, is_write_depth_buffer_enable_);
			MV1SetMeshBackCulling(mv_hdl_, 0, dxlib_culling_mode_);
			MV1SetSampleFilterMode(mv_hdl_, dxlib_sample_filter_mode_);
			MV1SetMeshDrawBlendMode(mv_hdl_, 0, dxlib_blend_mode_);
			MV1SetMeshDrawBlendParam(mv_hdl_, 0, i_alpha);
			MV1SetMatrix(mv_hdl_ , dxm);
			MV1DrawModel(mv_hdl_);
			SetTransformToWorld(&dxm);
		}

		//---------------------------------------------------------------------
		//
		// デバッグライン
		//
		SetWriteZBuffer3D(FALSE);
		SetUseZBuffer3D(FALSE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AXIS)			drawAxis();
		SetUseZBuffer3D(TRUE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_OBB)			drawOBB();
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_BD_SPHERE)	drawBdSphere();
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_VTX_NML)		drawVtxNormal();
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AABB)			drawAABB();
	}


	void Mesh::drawAxis() {
		tnl::Vector3 up(0, bd_sphere_radius_, 0);
		tnl::Vector3 right(bd_sphere_radius_, 0, 0);
		tnl::Vector3 depth(0, 0, bd_sphere_radius_);
		DrawLine3D({ 0, 0, 0 }, { up.x, up.y, up.z }, 0xff00ff00);
		DrawLine3D({ 0, 0, 0 }, { right.x, right.y, right.z }, 0xff0000ff);
		DrawLine3D({ 0, 0, 0 }, { depth.x, depth.y, depth.z }, 0xffff0000);
	}

	void Mesh::drawOBB() {
		float w = bd_box_size_.x * 0.5f;
		float h = bd_box_size_.y * 0.5f;
		float d = bd_box_size_.z * 0.5f;
		tnl::Vector3 v[8] = {
			{ -w,  h,  d }, {  w,  h,  d },  {  w,  h,  -d }, { -w,  h, -d },
			{ -w, -h,  d }, {  w, -h,  d },  {  w, -h,  -d }, { -w, -h, -d }
		};
		uint32_t col = 0xff66ff33;
		for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
		for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
		for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
		DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
		DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);
	}

	void Mesh::drawAABB() {
		tnl::Matrix mt_trans;
		tnl::Matrix mt_scale;
		tnl::Matrix mt_obj_world;

		// オブジェクトのワールド行列の作成
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);
		mt_scale = tnl::Matrix::Scaling(scl_.x, scl_.y, scl_.z);

		float w = bd_box_size_.x * 0.5f;
		float h = bd_box_size_.y * 0.5f;
		float d = bd_box_size_.z * 0.5f;
		tnl::Vector3 v[8] = {
			{ -w,  h,  d }, {  w,  h,  d },  {  w,  h,  -d }, { -w,  h, -d },
			{ -w, -h,  d }, {  w, -h,  d },  {  w, -h,  -d }, { -w, -h, -d }
		};
		for (int i = 0; i < 8; ++i) v[i] += pos_;
		uint32_t col = 0xff6633ff;

		MATRIX im;
		mt_obj_world = mt_scale * mt_trans;
		memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
		SetTransformToWorld(&im);
		for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
		for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
		for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
		DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
		DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);
	}

	void Mesh::drawBdSphere() {
		const int ring_vtx_num = 16;
		int s = ring_vtx_num - 1;
		int e = 0;

		tnl::Vector3 vx[ring_vtx_num];
		tnl::Vector3 vy[ring_vtx_num];
		tnl::Vector3 vz[ring_vtx_num];

		// X軸リング
		for (int i = 0; i < ring_vtx_num; ++i) {
			vx[i].x = 0;
			vx[i].y = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
			vx[i].z = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
		}
		for (int i = 0; i < (ring_vtx_num - 1); ++i) {
			DrawLine3D({ vx[i].x, vx[i].y, vx[i].z },
				{ vx[i + 1].x, vx[i + 1].y, vx[i + 1].z }, 0xffff0000);
		}
		DrawLine3D({ vx[s].x, vx[s].y, vx[s].z },
			{ vx[e].x, vx[e].y, vx[e].z }, 0xffff0000);


		// Y軸リング
		for (int i = 0; i < ring_vtx_num; ++i) {
			vy[i].x = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
			vy[i].y = 0;
			vy[i].z = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
		}
		for (int i = 0; i < (ring_vtx_num - 1); ++i) {
			DrawLine3D({ vy[i].x, vy[i].y, vy[i].z },
				{ vy[i + 1].x, vy[i + 1].y, vy[i + 1].z }, 0xff00ff00);
		}
		DrawLine3D({ vy[s].x, vy[s].y, vy[s].z },
			{ vy[e].x, vy[e].y, vy[e].z }, 0xff00ff00);

		// Z軸リング
		for (int i = 0; i < ring_vtx_num; ++i) {
			vz[i].x = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
			vz[i].y = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * bd_sphere_radius_;
			vz[i].z = 0;
		}
		for (int i = 0; i < (ring_vtx_num - 1); ++i) {
			DrawLine3D({ vz[i].x, vz[i].y, vz[i].z },
				{ vz[i + 1].x, vz[i + 1].y, vz[i + 1].z }, 0xff0000ff);
		}
		DrawLine3D({ vz[s].x, vz[s].y, vz[s].z },
			{ vz[e].x, vz[e].y, vz[e].z }, 0xff0000ff);
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
