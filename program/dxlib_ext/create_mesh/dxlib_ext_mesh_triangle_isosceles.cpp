#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateTriangleIsosceles(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = size_wh.length();
		mesh->bd_box_size_ = { size_wh.x, size_wh.y, 1.0f };

		// �����т̒��_�� = ( �������� * 2 ) - ( �������� - 1 )
		// �c���т̒��_�� = ( �c������ * 2 ) - ( �c������ - 1 )
		// �����_�� = �����т̒��_�� * �c���т̒��_��
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		float width = size_wh.x;
		float height = size_wh.y;

		int stack = div_h;
		int srice = div_w;
		//	tnl::Vector3 c = (tnl::Vector3(0, hh, 0) + tnl::Vector3(-ww, -hh, 0) + tnl::Vector3(ww, -hh, 0)) / 3.0f;
		tnl::Vector3 c;

		// ���_���W�EUV�E�@���̌v�Z
		tnl::Vector3 vv;
		for (int i = 0; i < (stack + 1); ++i) {
			for (int k = 0; k < (srice + 1); ++k) {
				vv.x = (-(width * 0.5f) + (width / srice * k)) * (1.0f / stack * i);
				vv.y = height - (height / stack * i);
				vv.z = 0;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 0.5f + (mesh->vtxs_[(i * (srice + 1)) + k].pos.x / width);
				mesh->vtxs_[(i * (srice + 1)) + k].v = 1.0f - (1.0f / (float)stack * i);

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



	Mesh* Mesh::CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateTriangleIsosceles(size_wh, div_w, div_h, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}

