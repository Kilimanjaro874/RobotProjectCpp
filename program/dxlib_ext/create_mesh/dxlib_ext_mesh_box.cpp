#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateBox(const float size, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = tnl::Vector3( size / 2, size / 2, size / 2).length();
		mesh->bd_box_size_ = { size / 2, size / 2, size / 2 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num * 6);


		float sx = size * 0.5f;
		float sy = size * 0.5f;
		float sz = size * 0.5f;

		float agls[6] = { 0, 270, 180, 90, 90, -90 };
		tnl::Vector3 axis[6] = { 
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::right,
			tnl::Vector3::right
		};

		for (int n = 0; n < 6; ++n) {

			int s_idx = vtx_num * n;
			tnl::Quaternion rq = tnl::Quaternion::RotationAxis(axis[n], tnl::ToRadian(agls[n]));

			for (int i = 0; i < (div_h + 1); ++i) {
				float v = i / ((float)div_h);
				float y = -sy + v * size;
				for (int k = 0; k < (div_w + 1); ++k) {
					float u = k / ((float)div_w);

					int ary = s_idx + (i * (div_w + 1)) + k;

					tnl::Vector3 p, nm;
					p.x = -sx + u * size;
					p.y = y;
					p.z = -sz;
					p = tnl::Vector3::TransformCoord(p, rq);
					nm = tnl::Vector3::TransformCoord({0, 0, -1}, rq);

					mesh->vtxs_[ary].pos = { p.x, p.y, p.z };
					if (is_left_cycle) {
						mesh->vtxs_[ary].norm = { nm.x, nm.y, nm.z };
					}
					else {
						mesh->vtxs_[ary].norm = { -nm.x, -nm.y, -nm.z };
					}


					mesh->vtxs_[ary].u = (float)n * (1.0f / 6.0f) + (u / 6.0f);
					mesh->vtxs_[ary].v = 1.0f - v;

					mesh->vtxs_[ary].dif = GetColorU8(255, 255, 255, 255);
				}
			}
		}


		int index_num = div_w * div_h * 6 ;
		mesh->idxs_.resize(index_num * 6);

		// 頂点インデックスの計算
		for (int n = 0; n < 6; ++n) {
			int s_idx = index_num * n;
			int v_idx = vtx_num * n;
			for (int i = 0; i < div_h; ++i) {
				for (int k = 0; k < div_w; ++k) {
					int a = s_idx + (i * div_w + k) * 6;

					if (is_left_cycle) {
						// 左上の三角形
						mesh->idxs_[a + 2] = v_idx + (i * (div_w + 1)) + k;
						mesh->idxs_[a + 1] = mesh->idxs_[a + 2] + 1;
						mesh->idxs_[a + 0] = mesh->idxs_[a + 2] + (div_w + 1);

						// 右下の三角形
						mesh->idxs_[a + 5] = mesh->idxs_[a + 0];
						mesh->idxs_[a + 4] = mesh->idxs_[a + 1];
						mesh->idxs_[a + 3] = mesh->idxs_[a + 5] + 1;

					}
					else {
						// 左上の三角形
						mesh->idxs_[a + 0] = v_idx + (i * (div_w + 1)) + k;
						mesh->idxs_[a + 1] = mesh->idxs_[a + 0] + 1;
						mesh->idxs_[a + 2] = mesh->idxs_[a + 0] + (div_w + 1);
						// 右下の三角形
						mesh->idxs_[a + 3] = mesh->idxs_[a + 2];
						mesh->idxs_[a + 4] = mesh->idxs_[a + 1];
						mesh->idxs_[a + 5] = mesh->idxs_[a + 3] + 1;
					}

				}
			}
		}

		mesh->createVBO();
		return mesh;
	}


	Mesh* Mesh::CreateBoxMV(const float size, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateBox(size, div_w, div_h, is_left_cycle);
		return CreateConvertMV(mesh);
	}


}

