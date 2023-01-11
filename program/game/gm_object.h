#pragma once
#include <memory>
#include "gm_coordinate.h"
#include "gm_assemble.h"

namespace tol {
	class Object {
	/// <summary>
	/// 
	/// </summary>
	public:
		Object() {}
		virtual ~Object() {
			delete assemble_;
		}
		// ---- General ---- //
		int id_;
		std::string name_;
	protected:
		// ---- Tree struct ---- //
		std::shared_ptr<Object> parent_;
		std::vector<std::weak_ptr<Object>> children_;

		// ---- General ---- //
		Coordinate coordinate_;				// have a
		Assemble* assemble_ = nullptr;		// have a (get from AssemRepo)
		
	public:
		
		virtual void update(float delta_time);
		virtual void render(dxe::Camera* camera);

	private:
	};
}