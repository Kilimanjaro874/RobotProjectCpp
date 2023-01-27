#include "gm_restraint.h"
#include "gm_object.h"
#include "gm_coordinate.h"

void tol::Restraint::init(const std::shared_ptr<Object> target, int re_id, std::string re_name, restraint_type type)
{
	restraint_st_ tmp;
	tmp.re_id_ = re_id;
	tmp.re_name_ = re_name;
	tmp.target_ = target;
	tmp.re_type_ = type;
	re_data_st_.push_back(tmp);
}

void tol::Restraint::update(float delta_time, std::shared_ptr<Object> obj)
{
	restraint(delta_time, obj);
}

void tol::Restraint::restraint(float delta_time, std::shared_ptr<Object> obj) {
	for (auto re : re_data_st_) {
		// --- Copy coordinate orientation from target Obj. --- //
		if (restraint_type::rot_as_rot == re.re_type_) {
			std::shared_ptr<Object> tar_obj = re.target_.lock();
			if (!tar_obj) {
				return; 
			}
			auto tar_cod = tar_obj->getCoordinate();
			auto cod = obj->getCoordinate();
			cod->setDirX(tar_cod->getDirX());
			cod->setDirX(tar_cod->getDirY());
			cod->setDirZ(tar_cod->getDirZ());
			cod->setRot(tar_cod->getRot());	 // reset rot;
		}
	}
}
