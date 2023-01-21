#include "gm_inverse_kinematics.h"

void tol::InvKinematics::init(const std::shared_ptr<Object> target, const std::shared_ptr<Object> object, 
	int ik_id, std::string ik_name, ik_type type, float kp, 
	bool is_x_rot, bool is_y_rot, bool is_z_rot)
{
	ik_data_st tmp;
	tmp.ik_id_ = ik_id;
	tmp.ik_name_ = ik_name;
	tmp.target_ = target;
	tmp.object_ = object;
	tmp.ik_type_ = type;
	tmp.kp_ = kp;
	tmp.is_rot_[static_cast<int>(Coordinate::coordinate::x)] = is_x_rot;
	tmp.is_rot_[static_cast<int>(Coordinate::coordinate::y)] = is_y_rot;
	tmp.is_rot_[static_cast<int>(Coordinate::coordinate::z)] = is_z_rot;
	ik_data_st_.push_back(tmp);
}

tnl::Quaternion tol::InvKinematics::update(float delta_time, std::shared_ptr<Object> obj) {
	return inverseKinematics(delta_time, obj);
}

tnl::Quaternion tol::InvKinematics::inverseKinematics(float delta_time, std::shared_ptr<Object> obj) {
	tnl::Quaternion rot_ik_one_flame = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, 0);
	if (ik_data_st_.size() == 0) {
		return rot_ik_one_flame;		// return non rot
	}
	tnl::Vector3 pe, pr, x, y;
	float dth = 0;
	std::shared_ptr<Coordinate> cod = obj->getCoordinate();
	for (auto ik : ik_data_st_) {
		for (int i = 0; i < static_cast<int>(Coordinate::coordinate::end); i++) {
			if (!ik.is_rot_[i]) {
				continue; 
			}
			// --- get ref. --- //
			std::shared_ptr<Object> object = ik.object_.lock();
			std::shared_ptr<Object> target = ik.target_.lock();
			if (object == nullptr || target == nullptr) {
				continue; 
			}
			std::shared_ptr<Coordinate> o_cod = object->getCoordinate();
			std::shared_ptr<Coordinate> t_cod = target->getCoordinate();
			
			switch (ik.ik_type_)
			{
			case(ik_type::pos_to_pos):
				pe = o_cod->getPos() - cod->getPos();
				pr = t_cod->getPos() - cod->getPos();
				break;
			
			case(ik_type::dirx_as_dirx):
				pe = o_cod->getDirX();
				pr = t_cod->getDirX();
				break;
			case(ik_type::dirx_as_diry):
				pe = o_cod->getDirX();
				pr = t_cod->getDirY();
				break;
			case(ik_type::dirx_as_dirz):
				pe = o_cod->getDirX();
				pr = t_cod->getDirZ();
				break;

			case(ik_type::diry_as_diry):
				pe = o_cod->getDirY();
				pr = t_cod->getDirY();
				break;
			case(ik_type::diry_as_dirx):
				pe = o_cod->getDirY();
				pr = t_cod->getDirX();
				break;
			case(ik_type::diry_as_dirz):
				pe = o_cod->getDirY();
				pr = t_cod->getDirZ();
				break;

			case(ik_type::dirz_as_dirz):
				pe = o_cod->getDirZ();
				pr = t_cod->getDirZ();
				break;
			case(ik_type::dirz_as_dirx):
				pe = o_cod->getDirZ();
				pr = t_cod->getDirX();
				break;
			case(ik_type::dirz_as_diry):
				pe = o_cod->getDirZ();
				pr = t_cod->getDirY();
				break;

			case(ik_type::dirx_look_pos):
				pe = o_cod->getDirX();
				pr = t_cod->getPos() - cod->getPos();
				break;
			case(ik_type::diry_look_pos):
				pe = o_cod->getDirY();
				pr = t_cod->getPos() - cod->getPos();
				break;
			case(ik_type::dirz_look_pos):
				pe = o_cod->getDirZ();
				pr = t_cod->getPos() - cod->getPos();
				break;
			default:

				break;
			}
			// --- get axis --- //
			tnl::Vector3 rot_axis;
			if (i == static_cast<int>(Coordinate::coordinate::x)) { rot_axis = cod->getDirX(); }
			if (i == static_cast<int>(Coordinate::coordinate::y)) { rot_axis = cod->getDirY(); }
			if (i == static_cast<int>(Coordinate::coordinate::z)) { rot_axis = cod->getDirZ(); }
			// --- calc ik ---- //
			x = tnl::Vector3::Cross(pe, rot_axis);
			y = tnl::Vector3::Cross(pr, rot_axis);
			dth = delta_time * 60.0 * ik.kp_ * std::acosf(std::clamp(
				x.dot(y) / x.length() / y.length(),
				(float)-1, (float)1
			));
			if (!isfinite(dth)) { dth = 0; }	// avoid singularity : when object or target exist on the rotation axis.
			if (dth > tnl::PI / 24) {			// limitter
				dth = tnl::PI / 24;
			}
			tnl::Vector3 axis = x.cross(y) / x.length() / y.length();	// determine rotate direction.
			dth *= rot_axis.dot(axis) >= 0 ? 1 : -1;
			rot_ik_one_flame *= tnl::Quaternion::RotationAxis(rot_axis, dth);
		}
	}
	return rot_ik_one_flame;
}
