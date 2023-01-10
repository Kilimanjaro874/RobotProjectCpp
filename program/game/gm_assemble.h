#pragma once
#include "../dxlib_ext/dxlib_ext.h"

namespace tol {
	class Assemble {
	/// <summary>
	/// Handle mech classes & attach to Object class.
	/// <relation>
	/// AssemStocker class : has this Classes.
	///  -> Object class: have a this Class. (copy & setting(ex. resize) from AssemStocker->this Class)
	/// update : calc direct kinematics on Object coordinate
	/// render : render mesh.
	/// </summary>
	public:
		Assemble() {}
		virtual ~Assemble() {
			for (auto m : mesh_v_) {
				delete m;
			}
		}
	private:
		bool is_update_ = true;					// flag: do update (calc. direct kinematics reffer to Object coordinate)
		bool is_render_ = true;					// flag: do render
		tnl::Vector3 offset_pos_;				// from coordinate origin.
		tnl::Quaternion rot_;					// this class rotation of : Object coordinate rot + this rot.
		std::vector<dxe::Mesh*> mesh_v_;		

	public:
		virtual void update(float delta_time);
		virtual void render(dxe::Camera* camera);
		// ---- setter ---- //
		void setMesh(dxe::Mesh* mesh) { mesh_v_.push_back(mesh); }
		void setIsUpdate(bool is_update) { is_update_ = is_update; }
		void setIsRender(bool is_render) { is_render_ = is_render; }
		void setOffset_pos(const tnl::Vector3& offset_pos) { offset_pos_ = offset_pos; }
		void setRot(const tnl::Quaternion& rot) { rot_ = rot; }
		void setScale(const tnl::Vector3& scale);
		// ---- getter ----- //

	};
}