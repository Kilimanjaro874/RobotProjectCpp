#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"
#include "gm_coordinate.h"

namespace tol {
	class Object;
	class Coordinate;

	class Restraint {

	public:
		Restraint() {}
		virtual ~Restraint() {}
		//// ---- Member variables ---- ////
		enum class restraint_type {
			// this - target //
			rot_as_rot,
		};

	protected:
		struct restraint_st_ {
			int re_id_;
			std::string re_name_;
			std::weak_ptr<Object> target_;
			restraint_type re_type_;
		};
		std::vector<restraint_st_> re_data_st_;
		bool is_re_init_ = false;
		//// ---- Member function ---- ////
	public:
		virtual void init(const std::shared_ptr<Object> target, int re_id, std::string re_name, restraint_type type);
		virtual void update(float delta_time, std::shared_ptr<Object> obj);
	private:
		void restraint(float delta_time, std::shared_ptr<Object> obj);
	};
}