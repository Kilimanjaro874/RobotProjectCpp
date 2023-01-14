#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_parts.h"
#include "gm_coordinate.h"

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
		virtual ~Assemble() {}
	private:
		bool is_update_ = true;					// flag: do update (calc. direct kinematics reffer to Object coordinate)
		bool is_render_ = true;					// flag: do render
		tnl::Vector3 offset_pos_;				// from coordinate origin.
		tnl::Quaternion rot_;					// this class rotation of : Object coordinate rot + this rot.	
		std::vector<std::shared_ptr<Parts>> parts_;

	public:
		virtual void update(float delta_time, const tnl::Vector3& coordinate_pos, const tnl::Quaternion& coordinate_rot);
		virtual void render(dxe::Camera* camera);
		std::vector<std::shared_ptr<Parts>> CreateCopyParts();	// return this parts(copied).

		// ---- setter ---- //
		void setParts(std::shared_ptr<Parts> parts) { parts_.push_back(parts); }
		void setIsUpdate(bool is_update) { is_update_ = is_update; }
		void setIsRender(bool is_render) { is_render_ = is_render; }
		void setOffset_pos(const tnl::Vector3& offset_pos) { offset_pos_ = offset_pos; }
		void setRot(const tnl::Quaternion& rot) { rot_ = rot; }
		void setScale(float scale);
		// ---- getter ----- //
		bool getIsUpdate() { return is_update_; }
		bool getIsRender() { return is_render_; }
		tnl::Vector3 getOffsetPos() { return offset_pos_; }
		tnl::Quaternion getRot() { return rot_; }
	};
}