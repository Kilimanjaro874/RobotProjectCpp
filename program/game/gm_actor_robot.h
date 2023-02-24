#pragma once

#include "gm_actor.h"

namespace tol {

	class RobotActor : public Actor {
	public:
		RobotActor(int id, std::string name) : Actor(id, name) {}
		virtual ~RobotActor() {}
		//// ---- Member variables ---- ////
	private:
		// --- define --- //
		// -- weapons controls -- //
		bool is_right_weapon_fire_ = false;		// Right weapon fire trigger
		bool is_left_weapon_fire_ = false;		// Left weapon fire trigger
		std::shared_ptr<Object> right_arm_target_ = nullptr;	// aim target
		std::shared_ptr<Object> left_arm_target_ = nullptr;		// aim target




	public:
		void init() override;
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
	};
}
