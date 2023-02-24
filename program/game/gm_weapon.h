#pragma once


namespace tol {
	
	class Weapon {
	public:
		Weapon() {}
		virtual ~Weapon() {}

		//// ---- Member variables ---- ////


	public:
		enum class bullet_type {
			machine_gun, rifle, bomb, end
		};
		enum class fire_dir {
			front, up, right
		};

	private:
		// --- define --- //
		bool is_fire_ = false;
		float reload_time_;
		float bullet_damage_;
		float bullet_speed_;
		float bullet_size_;

		float reload_count_;

		//// ---- Member functions ---- ////
	public:

	private:

	};
}