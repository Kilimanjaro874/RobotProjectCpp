#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateSphere(const float radius, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = radius;
		mesh->bd_box_size_ = { radius * 2,  radius * 2, radius * 2 };

		// ‰¡•À‚Ñ‚Ì’¸“_” = ( ‰¡•ªŠ„” * 2 ) - ( ‰¡•ªŠ„” - 1 )
		// c•À‚Ñ‚Ì’¸“_” = ( c•ªŠ„” * 2 ) - ( c•ªŠ„” - 1 )
		// ‘’¸“_” = ‰¡•À‚Ñ‚Ì’¸“_” * c•À‚Ñ‚Ì’¸“_”
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		// ’¸“_À•W‚ÌŒvZ
		for (int i = 0; i < (div_h + 1); ++i) {
			float v = i / ((float)div_h);
			float y = cosf(tnl::PI * v) * radius;
			float r = sinf(tnl::PI * v) * radius;

			// “ª’¸•”‚Ì’¸“_‚ªd•¡‚·‚é‚Ì‚ğ–h‚®‚½‚ß ( –@ü‚ªŒvZ‚Å‚«‚È‚­‚È‚é‚Ì‚Å )
			r = (r < FLT_EPSILON) ? 0.00001f : r;

			for (int k = 0; k < (div_w + 1); ++k) {
				float u = k / ((float)div_w);

				tnl::Vector3 pv;
				pv.x = cosf(2 * tnl::PI * u) * r;
				pv.y = y;
				pv.z = sinf(2 * tnl::PI * u) * r;

				mesh->vtxs_[(i * (div_w + 1)) + k].pos.x = pv.x;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.y = pv.y;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.z = pv.z;

				pv.normalize();

				if (is_left_cycle) {
					mesh->vtxs_[(i * (div_w + 1)) + k].norm = { pv.x, pv.y, pv.z };
				}
				else {
					mesh->vtxs_[(i * (div_w + 1)) + k].norm = { -pv.x, -pv.y, -pv.z };
				}


				mesh->vtxs_[(i * (div_w + 1)) + k].u = u;
				mesh->vtxs_[(i * (div_w + 1)) + k].v = v;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
				mesh->vtxs_[(i * (div_w + 1)) + k].spc = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;

	}


	Mesh* Mesh::CreateSphereMV(const float radius, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateSphere(radius, div_w, div_h, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
