#pragma once
#include <memory>
#include "gm_object.h"
#include "gm_assemble_repository.h"
#include "gm_physics_handler.h"

namespace tol {
	class AssemRepo;
	class PhysicsHandler;

	class Actor : public Object {
		
	public:
		Actor(int id, std::string name) : Object(id, name) {
			id_ = id; name_ = name;
		}
		virtual ~Actor() {}
		//// ---- Member variables ---- ////
	private:
		std::shared_ptr<PhysicsHandler> ph_handler_ = nullptr;
		//// ---- Member functions ---- ////
	public:
		void init() override;
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
		static std::shared_ptr<Actor> Create(std::shared_ptr<AssemRepo> a_repo, std::string csv_path, std::string ik_csv_path = "");
		// ---- setter ---- //
		void setPhysicsHandler(std::shared_ptr<PhysicsHandler> ph_handler) { ph_handler_ = ph_handler; }
		// ---- getter ---- //
		std::shared_ptr<PhysicsHandler> getPhysicsHandler() { return ph_handler_; }
	private:
		void getObjectDataCSV(std::shared_ptr<AssemRepo> a_repo, std::string csv_path);
		void getIKsettingDataCSV(std::string csv_path);
	};
}