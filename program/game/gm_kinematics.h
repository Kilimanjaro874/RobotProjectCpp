#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"
#include "gm_inverse_kinematics.h"

namespace tol {
	class Object;
	class InvKinematics;

	class Kinematics {
	public:
		Kinematics() {}
		virtual ~Kinematics() {}
	//// ---- Member variables ---- ////
	private:
		// --- for DK package --- //
		struct dk_data_st {		// store coordinate difference info. (this - parent)
			tnl::Vector3 dir_c_p_;
			float len_c_p_;
			tnl::Quaternion rot_;
		};
		dk_data_st dk_data_st_ = { {0, 0, 0}, 1.0 , tnl::Quaternion::RotationAxis({0, 1, 0}, 0) };
		tnl::Vector3 dk_pos_parent_to_this_ = { 0, 0, 0 };		 
		bool is_dk_init_ = false;								// dk_data_st_ init flag
		tnl::Quaternion rot_one_flame_;							// rotation of 1 flame.
		// --- If you need Inverse Kinematics(IK) : attach IKclass --- //
		std::shared_ptr<InvKinematics> inv_kinematics_ = nullptr;
		// --- If you need Easing move : attach Easing Class ---- //
		
	//// ---- Member functions ---- ////
	public:
		virtual void init(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child);
		virtual void update(float delta_time, std::shared_ptr<Object> obj);
		// ---- for user functions ---- //
		virtual void Translate(std::shared_ptr<Object> obj, tnl::Vector3 move, bool absolute_move);
		virtual void Rotation(std::shared_ptr<Object> obj, tnl::Quaternion rot, bool absolute_move);
	private:
		void initDKSetting(const std::shared_ptr<Object> parent, const std::shared_ptr<Object> child);
		void doKinematics(float delta_time, std::shared_ptr<Object> obj);
	public:
		// ---- setter ---- //
		void setInvKinematics(std::shared_ptr<InvKinematics> ik) { inv_kinematics_ = ik; }
		void setDkData(const tnl::Vector3& dir, float length) {
			dk_data_st_.dir_c_p_ = dir;
			dk_data_st_.len_c_p_ = length;
		}
		void setRotDkData(const tnl::Quaternion rot) {
			dk_data_st_.rot_ *= rot;
			dk_pos_parent_to_this_ = tnl::Vector3::TransformCoord(dk_data_st_.dir_c_p_ * dk_data_st_.len_c_p_, dk_data_st_.rot_);
		}
		void setRotOneFlame(tnl::Quaternion rot) { rot_one_flame_ = rot; }
		void setAddRotOneFlame(tnl::Quaternion rot) { rot_one_flame_ *= rot; }
		// ---- getter ---- //
		std::shared_ptr<InvKinematics> getInvKinematics() { return inv_kinematics_; }
		tnl::Quaternion getRotOneFlame(){ return rot_one_flame_; }

	};
}