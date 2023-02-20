#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	class Object;
	class Bullet;

	class Weapon : public std::enable_shared_from_this<Weapon> {
	public:
		Weapon(
			std::shared_ptr<Object> parent,
			std::shared_ptr<Assemble> bullet_assem,
			float reload_time, float bullet_damage,
			float bullet_speed, float bullet_size
		);
		virtual ~Weapon() {}
		//// ---- Member variables ---- ////
		enum class bullet_type
		{
			machine_gun, rifle, missile, bomb, end
		};
	private:
		// --- define --- //
		bool is_fire_ = false;
		std::weak_ptr<Object> parent_;		// ref. parent that has this component.
		float reload_time_;
		float reload_count_;
		float bullet_damage_;
		float bullet_speed_;
		float bullet_size_;
		bullet_type bullet_type = bullet_type::end;					// select bullet type
		std::shared_ptr<Assemble> bullet_assem_ = nullptr;			// for render 
		std::list<std::shared_ptr<tol::Weapon::Bullet>> bullets_;	// Manage bullet classes.
		bool is_fire_permit_ = false;
		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time);
		// ---- setter ---- //
		void setFirePermission(bool isfire) { is_fire_permit_ = isfire; }
		// ---- getter ---- //
		std::list<std::shared_ptr<tol::Weapon::Bullet>> getBullets() { return bullets_; }
	private:
		void genBullet();

		//////////////////////////////////////
		//// ---- have Bullet class ---- ////
		class Bullet {
		public:
			Bullet(
				tnl::Vector3 pos, 
				tnl::Vector3 dir, 
				float bullet_damage,
				float bullet_speed
			);
			virtual ~Bullet() {}
			//// ---- Member variables ---- ////
		private:
			bool is_hit_ = false;
			tnl::Vector3 pos_;
			tnl::Vector3 forward_dir_;
			tnl::Quaternion rot_;
			float bullet_damage_;
			float bullet_speed_;

			//// Member functions ---- ////
			virtual void update(float delta_time);
			virtual void render(dxe::Camera* camera);
		public:
			// ---- setter ---- //
			// ---- getter ---- //
		};
		////////////////////////////////////////
	};
}