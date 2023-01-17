#pragma once
#include "gm_parts.h"
#include "gm_assemble.h"

namespace tol {
	class Object;
	class Assemble {
		
	public:
		Assemble() {}
		virtual ~Assemble() {}
	private:
		bool is_render = true;
		tnl::Vector3 ofs_pos_;
		tnl::Quaternion ofs_rot_;
		std::vector<std::shared_ptr<Parts>> parts_;
	public:
		virtual void update(float delta_time, std::shared_ptr<Object> obj);
		virtual void render(dxe::Camera* camera);
		std::vector<std::shared_ptr<Parts>> CopyParts();
		// ---- setter ---- //
		void setParts(std::shared_ptr<Parts> parts) { parts_.push_back(parts); }
		void setPartsScale(float scale);
		void setOffsetPos(const tnl::Vector3& ofs_pos) { ofs_pos_ = ofs_pos; }
		void setOffsetRot(const tnl::Quaternion& ofs_rot) { ofs_rot_ = ofs_rot; }
		// ---- getter ---- //
	};
}