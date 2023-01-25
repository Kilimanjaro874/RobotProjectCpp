#pragma once
#include <memory>
#include "gm_object.h"
#include "gm_assemble_repository.h"
#include "gm_physics_handler.h"
#include "gm_pid_vel_controller.h"
#include "gm_pid_pos_controller.h"

namespace tol {
	class AssemRepo;
	class PhysicsHandler;
	class PIDVelController;
	class PIDPosController;

	class Actor : public Object {
		
	public:
		Actor(int id, std::string name) : Object(id, name) {
			id_ = id; name_ = name;
		}
		virtual ~Actor() {}
		//// ---- Member variables ---- ////
	private:
		// --- handle physics --- //
		std::shared_ptr<PhysicsHandler> ph_handler_ = nullptr;
		// --- velocity pid control --- //
		std::shared_ptr<PIDVelController> pid_vel_cont_ = nullptr;
		// --- position pid control --- //
		std::shared_ptr<PIDPosController> pid_pos_cont_ = nullptr;
		//// ---- Member functions ---- ////
	public:
		void init() override;
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
		static std::shared_ptr<Actor> Create(std::shared_ptr<AssemRepo> a_repo, std::string csv_path = "", std::string ik_csv_path = "");
		// ---- for user functions ---- //
		void operateActorUpdate(float delta_time, const tnl::Vector3& dir);		// give player controll effect.
		// ---- setter ---- //
		void setPhysicsHandler(std::shared_ptr<PhysicsHandler> ph_handler) { ph_handler_ = ph_handler; }
		void setPIDVelController(std::shared_ptr<PIDVelController> pid_vel_cont) { pid_vel_cont_ = pid_vel_cont; }
		void setPIDPosController(std::shared_ptr<PIDPosController> pid_pos_cont) { pid_pos_cont_ = pid_pos_cont; }
		// ---- getter ---- //
		std::shared_ptr<PhysicsHandler> getPhysicsHandler() { return ph_handler_; }
	private:
		void getObjectDataCSV(std::shared_ptr<AssemRepo> a_repo, std::string csv_path);
		void getIKsettingDataCSV(std::string csv_path);
	};
}