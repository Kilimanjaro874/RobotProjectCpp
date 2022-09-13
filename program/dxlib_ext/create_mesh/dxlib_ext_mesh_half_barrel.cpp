#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateHalfBarrel(const float radius, const float height, const float bulge, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
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
			float r = radius;
			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (180.0f / angle);

				tnl::Vector3 v;

				v.x = (cosf(angle_ofs + 2 * tnl::PI * u)) * r;
				v.y = height - (height / stack * i);
				v.z = (sinf(angle_ofs + 2 * tnl::PI * u)) * r;

				v.x *= 1.0f + sinf(tnl::PI * 0.5f * (height / stack * i) / height ) * (bulge - 1.0f);
				v.z *= 1.0f + sinf(tnl::PI * 0.5f * (height / stack * i) / height ) * (bulge - 1.0f);


				mesh->vtxs_[(i * (srice + 1)) + k].pos = { v.x, v.y, v.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				v = v.xz();
				v.normalize();
				if (is_left_cycle) {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { v.x, v.y, v.z };
				}
				else {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { -v.x, -v.y, -v.z };
				}



				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;

	}


	Mesh* Mesh::CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateHalfBarrel(radius, height, bulge, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
