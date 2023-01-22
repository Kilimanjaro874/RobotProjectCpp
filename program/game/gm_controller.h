#pragma once
#include<memory>

namespace tol {
	class Actor;
	class InputHandler;
	class Commando;
	
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


	};

	class Commando {
	public:
		Commando() {}
		virtual ~Commando() {}
		virtual void execute(std::shared_ptr<Actor> actor) = 0;
	};
}