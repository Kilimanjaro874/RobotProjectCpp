#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreatePlane(
		const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle,
		const tnl::Vector3& ltop_uv, const tnl::Vector3 rbottom_uv ) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = size_wh.length() / 2;
		mesh->bd_box_size_ = { size_wh.x, size_wh.y, 1.0f };

		// ‰¡•À‚Ñ‚Ì’¸“_” = ( ‰¡•ªŠ„” * 2 ) - ( ‰¡•ªŠ„” - 1 )
		// c•À‚Ñ‚Ì’¸“_” = ( c•ªŠ„” * 2 ) - ( c•ªŠ„” - 1 )
		// ‘’¸“_” = ‰¡•À‚Ñ‚Ì’¸“_” * c•À‚Ñ‚Ì’¸“_”
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		float sx = size_wh.x * 0.5f;
		float sy = size_wh.y * 0.5f;

		float u_size = rbottom_uv.x - ltop_uv.x;
		float v_size = rbottom_uv.y - ltop_uv.y;

		for (int i = 0; i < (div_h + 1); ++i) {
			float v = i / ((float)div_h);
			float y = -sy + v * size_wh.y;

			for (int k = 0; k < (div_w + 1); ++k) {
				float u = k / ((float)div_w);
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.x = -sx + u * size_wh.x;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.y = y;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.z = 0;

				mesh->vtxs_[(i * (div_w + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (div_w + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (div_w + 1)) + k].norm.z = (is_left_cycle)? -1.0f : 1.0f;

				//mesh->vtxs_[(i * (div_w + 1)) + k].u = u;
				//mesh->vtxs_[(i * (div_w + 1)) + k].v = 1.0f - v;
				mesh->vtxs_[(i * (div_w + 1)) + k].u = ltop_uv.x + ( u * u_size );
//				mesh->vtxs_[(i * (div_w + 1)) + k].v = ( 1.0f - ltop_uv.y ) + ( ( 1.0f - v ) * v_size );
				mesh->vtxs_[(i * (div_w + 1)) + k].v = 1.0f - ( ltop_uv.y + ( v * v_size ) );


				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;
	}


	Mesh* Mesh::CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle,
		const tnl::Vector3& ltop_uv, const tnl::Vector3 rbottom_uv ) noexcept {
		Mesh* mesh = CreatePlane(size_wh, div_w, div_h, is_left_cycle, ltop_uv, rbottom_uv);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}

