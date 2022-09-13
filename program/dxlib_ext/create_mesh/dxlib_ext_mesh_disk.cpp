#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateDisk(const float radius, const int div_w, const int div_h, const int angle, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		mesh->bd_sphere_radius_ = radius;
		mesh->bd_box_size_ = { radius * 2,  radius * 2, radius * 2 };

		uint32_t deg = 360;
		deg = (angle < 360) ? 180 : deg;
		deg = (angle < 180) ? 90 : deg;
		deg = (angle < 90) ? 45 : deg;

		// �����т̒��_�� = ( �������� * 2 ) - ( �������� - 1 )
		// �c���т̒��_�� = ( �c������ * 2 ) - ( �c������ - 1 )
		// �����_�� = �����т̒��_�� * �c���т̒��_��
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack);
			if (360 > deg) v *= 0.5f;

			float y = cosf(tnl::PI * v) * radius;
			float r = sinf(tnl::PI * v) * radius;
			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				if (90 >= deg) u *= 0.5f;
				tnl::Vector3 vv = tnl::Vector3(cosf(tnl::PI * u) * r, y, 0);

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 0.5f + (vv.x / radius * 0.5f);
				mesh->vtxs_[(i * (srice + 1)) + k].v = (0.5f + (vv.y / radius * 0.5f));

				mesh->vtxs_[(i * (srice + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.z = ( is_left_cycle ) ? -1.0f : 1.0f ;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;

	}


	Mesh* Mesh::CreateDiskMV(const float radius, const int div_w, const int div_h, const int angle, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateDisk(radius, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
