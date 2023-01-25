#pragma once
#include<memory>
#include "gm_actor.h"
#include "gm_physics_handler.h"
#include "gm_coordinate.h"

namespace tol {
	class Actor;
	class InputHandler;
	class Commando;
	class PhysicsHandler;
	class Coordinate;
	
	class Controller {
	public:
		Controller() {}
		virtual ~Controller() {}

	protected:
		std::shared_ptr<InputHandler> input_handler_ = nullptr;
	public:
		virtual void init();
		virtual void update(float delta_time, std::shared_ptr<Actor> actor);
	};

	class InputHandler {
	public:
		std::shared_ptr<Commando> MoveHorizontal;
		std::shared_ptr<Commando> Horizontal;
		std::shared_ptr<Commando> handleInput(float delta_time);

	};

	// ---- Abstruct ---- //
	class Commando {
	public:
		Commando() {}
		virtual ~Commando() {}
		virtual void execute(float delta_time, std::shared_ptr<Actor> actor) = 0;
	};

	// ---- implement abstract classes ---- //
	class MoveRightCommand : public Commando {
	/// <summary>
	/// For horizontal movement fuc.
	/// * Actor : Physics_handler must be initialized and set in advance
	/// </summary>
	public:
		/// <summary>
		/// vel_dir : tnl::Vector(x, 0, z) [-1 <= x <= 1, -1 <= z <= 1]
		/// </summary>
		virtual void execute(float delta_time, std::shared_ptr<Actor> actor) {
			std::shared_ptr<PhysicsHandler> ph_h = actor->getPhysicsHandler();
			std::shared_ptr<Coordinate> cod = actor->getCoordinate();
			//tnl::Vector3 tmp_dir = tnl::Vector3::TransformCoord(vel_dir, cod->getRot());
			if (ph_h) {
				//ph_h->update(delta_time, tmp_dir);
			}
		}
	};

	class MoveVerticalCommand : public Commando {
		/// <summary>
		/// For vertical movement func.
		/// * Actor : Physics_handler must be initialized and set in advance
		/// </summary>
	public:
		/// <summary>
		/// vel_dir : tnl::Vector(0, y, 0) [-1 <= y <= 1]
		/// </summary>
		virtual void execute(float delta_time, std::shared_ptr<Actor> actor, const tnl::Vector3& vel_dir) {
			std::shared_ptr<PhysicsHandler> ph_h = actor->getPhysicsHandler();
			std::shared_ptr<Coordinate> cod = actor->getCoordinate();
			tnl::Vector3 tmp_dir = tnl::Vector3::TransformCoord(vel_dir, cod->getRot());
			if (ph_h) {
				//ph_h->update(delta_time, tmp_dir);
			}
		}
	};
}