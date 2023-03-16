#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Actor;

	class CircleCollider {
	public: 
		CircleCollider(float radius) {
			radius_ = radius;
		}
		virtual ~CircleCollider() {}

		//// ---- Member variables ---- ////
	protected:
		// --- define --- //
		float radius_ = 0.0f;
		bool is_hit_ = false;
		std::shared_ptr<Object> hit_object_ = nullptr;
		//// ---- Member functions ---- ////
	public:
		void update(float delta_time);
		virtual void hitCheck(std::shared_ptr<Object> this_obj, std::shared_ptr<Object> other_obj);

		// ---- getter ---- //
		float getCircleSize() { return radius_; }
		bool getIsHit() { return is_hit_; }
		std::shared_ptr<Object> getHitObject() { return hit_object_; }
	};
}