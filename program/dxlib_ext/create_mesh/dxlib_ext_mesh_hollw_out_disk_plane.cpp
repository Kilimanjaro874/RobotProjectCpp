#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateHollowOutDiskPlane(const float radius, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = radius;
		mesh->bd_box_size_ = { radius * 2,  radius * 2, radius * 2 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (stack + 1); ++i) {

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				tnl::Vector3 vv(0, 0, 0);
				float rd = 2 * tnl::PI * u;

				vv.x = (cosf(rd) * radius);
				vv.y = (sinf(rd) * radius);

				if (i == stack) {
					vv = tnl::Vector3::Normalize(vv) * radius * 2;
					vv.x = (vv.x > radius) ? radius : vv.x;
					vv.y = (vv.y > radius) ? radius : vv.y;
					vv.x = (vv.x < -radius) ? -radius : vv.x;
					vv.y = (vv.y < -radius) ? -radius : vv.y;
				}

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 0.5f + (vv.x / radius * 0.5f);
				mesh->vtxs_[(i * (srice + 1)) + k].v = (0.5f + (vv.y / radius * 0.5f));

				mesh->vtxs_[(i * (srice + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.z = (is_left_cycle)? -1.0f : 1.0f;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;

	}

	Mesh* Mesh::CreateHollowOutDiskPlaneMV(const float radius, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateHollowOutDiskPlane(radius, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
