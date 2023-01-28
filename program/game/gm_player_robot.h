#pragma once

#include "gm_actor.h"

namespace tol {

	class PlayerRobo : public Actor {
	public:
		PlayerRobo(int id, std::string name) : Actor(id, name) {}
		virtual ~PlayerRobo() {}
		//// ---- Member variables ---- ////
	private:
		// --- for game 

	public:
		void update(float delta_time) override;


	};
}
