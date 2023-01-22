#pragma once
#include <memory>
#include "../dxlib_ext/dxlib_ext.h"

namespace tol {
	class Actor;
	class PIDController;

	class PhysicsHandler {
	public:
		PhysicsHandler() {}
		virtual ~PhysicsHandler() {}
	private:
		//// ---- Member variables ---- ////
		// --- define --- //
		float mass_;			// kg
		
		// --- for PID Controll --- //
		std::shared_ptr<PIDController> pid_cont_ = nullptr;

		//// ---- Member functions ---- ////

	};

	class PIDController {
	public:



	};
}