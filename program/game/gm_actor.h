#pragma once
#include <memory>
#include "gm_object.h"
#include "gm_assemble_repository.h"
#include "gm_controller.h"

namespace tol {
	class Controller;

	class Actor : public Object {
		
	public:
		Actor(int id, std::string name) : Object(id, name) {
			id_ = id; name_ = name;
		}
		virtual ~Actor() {}

		//// ---- Member variables ---- ////
	private:
		
		//// ---- Member functions ---- ////
	public:
		void init() override;
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
		static std::shared_ptr<Actor> Create(std::shared_ptr<AssemRepo> a_repo, std::string csv_path, std::string ik_csv_path = "");
	private:
		void getObjectDataCSV(std::shared_ptr<AssemRepo> a_repo, std::string csv_path);
		void getIKsettingDataCSV(std::string csv_path);
	};
}