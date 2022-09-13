#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateConeCylinder(const float top_radius, const float bottom_radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Mesh* mesh = new Mesh();

		float max = (bottom_radius > height) ? bottom_radius : height;
		mesh->bd_sphere_radius_ = max;
		mesh->bd_box_size_ = { max * 2,  max * 2, max * 2 };

		// �����т̒��_�� = ( �������� * 2 ) - ( �������� - 1 )
		// �c���т̒��_�� = ( �c������ * 2 ) - ( �c������ - 1 )
		// �����_�� = �����т̒��_�� * �c���т̒��_��
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		float btr = bottom_radius - top_radius;

		// ���_���W�EUV�E�@���̌v�Z
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack) * 0.5f;
			float y = cosf(tnl::PI * v) * btr ;
			float r = top_radius + (1.0f / (float)stack) * (btr - y) * btr * 0.5f;
			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			// �������̒��_���d������̂�h������ ( �@�����v�Z�ł��Ȃ��Ȃ�̂� )
			r = (r < FLT_EPSILON) ? 0.00001f : r;

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				tnl::Vector3 vv;
				vv.x = (cosf(angle_ofs + 2 * tnl::PI * u)) * r;
				vv.y = y * (height / btr);
				vv.z = (sinf(angle_ofs + 2 * tnl::PI * u)) * r;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				vv.normalize();
				if (is_left_cycle) {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { vv.x, vv.y, vv.z };
				}
				else {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { -vv.x, -vv.y, -vv.z };
				}


				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();
		return mesh;

	}


	Mesh* Mesh::CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateConeCylinder(top_radius, bottom_radius, height, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		float max = (bottom_radius > height) ? bottom_radius : height;
		mesh->bd_sphere_radius_ = max;
		mesh->bd_box_size_ = { max * 2,  max * 2, max * 2 };
		return mesh;
	}


}
