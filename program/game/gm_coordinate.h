#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace tol {
	class Coordinate {
		/// <summary>
		/// Handle pos and rotate information.
		/// <reration>
		/// Ojject class : have a this Class.
		/// </summary>
	public:
		Coordinate() {}
		virtual ~Coordinate() {
			for (auto m : mesh_v_) {
				delete m;
			}
		}

	private:
		// ---- General ---- //
		tnl::Vector3 pos_;			// origine pos.
		tnl::Quaternion rot_;		// rotate 
		enum class coordinate {
			x, y, z, end
		};
		std::vector<tnl::Vector3>
			oc_vec_v_,				// orthonormal basis(initial value)
			oc_vec_upd_v_;			// (update value)

		// ---- Render ---- //
		bool is_render_ = false;
		std::vector<dxe::Mesh*> mesh_v_;

	public:
		virtual void update(float delta_time);
		virtual void render(dxe::Camera* camera);
		void setCoordinate(
			tnl::Vector3 pos = {0, 0, 0},
			tnl::Quaternion rot = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0),
			tnl::Vector3 dir_x = {1, 0, 0},
			tnl::Vector3 dir_y = {0, 1, 0},
			tnl::Vector3 dir_z = {0, 0, 1}
		);
		void setViewCoordinate(float length, float radius);
		// ---- setter ---- //

		// ---- getter ---- //
		tnl::Vector3 getPos() { return pos_; }
		tnl::Quaternion getRot() { return rot_; }
		tnl::Vector3 getDirX() { return oc_vec_upd_v_[static_cast<int>(coordinate::x)]; }
		tnl::Vector3 getDirY() { return oc_vec_upd_v_[static_cast<int>(coordinate::y)]; }
		tnl::Vector3 getDirZ() { return oc_vec_upd_v_[static_cast<int>(coordinate::z)]; }
	};
}