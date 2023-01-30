#pragma once

#include "gm_actor.h"

namespace tol {

	class PlayerRobo : public Actor {
	public:
		PlayerRobo(int id, std::string name) : Actor(id, name) {}
		virtual ~PlayerRobo() {}
		//// ---- Member variables ---- ////
	private:
		// --- define --- //
		bool is_right_weapon_fire_ = false;		// Right weapon fire trigger
		bool is_left_weapon_fire_ = false;		// Left weapon fire trigger
		tnl::Vector3 aim_right_target_ = { 0, 0, 0 };
		tnl::Vector3 aim_left_target_ = { 0, 0, 0 };

	public:
		void update(float delta_time) override;
	};
}
