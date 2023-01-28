#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_actor.h"
#include "gm_camera.h"

namespace tol {
	class Commando;

	class InputHandler {
	public:
		InputHandler() {}
		virtual ~InputHandler() {}
		void init();
		std::shared_ptr<Commando> handleInput();
	private:
		std::shared_ptr<Commando> button_right_;
		std::shared_ptr<Commando> button_left_;
		std::shared_ptr<Commando> button_up_;
		std::shared_ptr<Commando> button_down_;

	};

	class Commando {
	public:
		virtual ~Commando() {}
		virtual void execute(float delta_time, Actor& actor, GmCamera& camera) = 0;
	};
	class StickMove : public Commando {


	};
	class MoveRight : public Commando {
	public:
		virtual void execute(float delta_time, Actor& actor, GmCamera& camera) {
			actor.pidVellContUpdate(delta_time, { 1.0, 0, 0 });
		}
	};

	class MoveLeft : public Commando {
	public:
		virtual void execute(float delta_time, Actor& actor, GmCamera& camera) {
			actor.pidVellContUpdate(delta_time, { -1.0, 0, 0 });
		}
	};

	class MoveFront : public Commando {
	public :
		virtual void execute(float delta_time, Actor& actor, GmCamera& camera) {
			actor.pidVellContUpdate(delta_time, { 0, 0, 1.0 });
		}
	};

	class MoveBack : public Commando {
	public:
		virtual void execute(float delta_time, Actor& actor, GmCamera& camera) {
			actor.pidVellContUpdate(delta_time, { 0, 0, -1.0 });
		}
	};
}