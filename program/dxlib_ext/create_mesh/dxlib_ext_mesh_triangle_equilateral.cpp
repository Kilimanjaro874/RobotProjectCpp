#include "../dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Mesh* Mesh::CreateTriangleEquilateral(const float edge_length, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{		
		float width = edge_length;
		float height = edge_length * sqrtf(3.0f) * 0.5f;
		Mesh* mesh = CreateTriangleIsosceles({ width, height, 0 }, div_w, div_h, is_left_cycle);
		mesh->bd_sphere_radius_ = edge_length;
		mesh->bd_box_size_ = { edge_length, edge_length, 1.0f };
		return mesh;
	}

	Mesh* Mesh::CreateTriangleEquilateralMV(const float edge_lenght, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Mesh* mesh = CreateTriangleEquilateral(edge_lenght, div_w, div_h, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}

