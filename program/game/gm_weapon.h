#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_object.h"

namespace tol {
	
	
	class Weapon {
	public:
		enum class fire_dir {
			front, up, right
		};
		enum class bullet_dir {
			front, up, right
		};
		Weapon(float reload_time, float bullet_damage, float bullet_speed, float bullet_effective_range, float bullet_hit_size,
			fire_dir fire_dir, bullet_dir bullet_dir,
			std::shared_ptr<Object> bullet_obj
			) {
			reload_time_ = reload_time;
			bullet_damage_ = bullet_damage;
			bullet_speed_ = bullet_speed;
			bullet_effective_range_ = bullet_effective_range;	// Maximum flight distance
			bullet_hit_size_ = bullet_hit_size;					// sphere radius.
			fire_dir_ = fire_dir;
			bullet_dir_ = bullet_dir;
			bullet_ = bullet_obj;
		}
		virtual ~Weapon() {}

		//// ---- Member variables ---- ////
	private:
		// --- define --- //
		bool is_fire_ = false;
		bool is_fired_ = false;
		float reload_time_;
		float bullet_damage_;
		float bullet_speed_;
		float bullet_effective_range_;
		float bullet_hit_size_;
		fire_dir fire_dir_ = fire_dir::front;
		bullet_dir bullet_dir_ = bullet_dir::front;
		// --- objects --- //
		std::shared_ptr<Object> bullet_ = nullptr;
		std::list<std::shared_ptr<Object>> bullets_;
		// --- nums --- //
		float reload_count_ = 0;
		// --- flags --- //
		bool is_alive_ = true;
		//// ---- Member functions ---- ////
	public:
		virtual void update(float delta_time, std::shared_ptr<Object> obj);
		virtual void render(dxe::Camera* camera);
		// ---- setter ---- //
		void setFire(bool is_fire) { is_fire_ = is_fire; }
		// ---- getter ---- //
		bool getFire() { return is_fire_; }
		bool getFired() { return is_fired_; }
		bool isAlive() { return is_alive_; }
		std::list<std::shared_ptr<Object>> getBullets() { return bullets_; }
	private:
		void genBullet(std::shared_ptr<Object> obj);
	};
}