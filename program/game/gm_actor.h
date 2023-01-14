#pragma once
#include <memory>
#include "gm_object.h"
#include "gm_assemble_repository.h"

namespace tol {
	class Actor : public Object {
	public:
		Actor(int id, std::string name) : Object(id, name) {
			id_ = id;
			name_ = name;
		}
		~Actor() {};

	public:
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
		static std::shared_ptr<Actor> Create(std::shared_ptr<AssemRepo> assem_repo, std::string csv_path);
	private:
		void getObjectDataCSV(std::shared_ptr<AssemRepo> assem_repo, std::string csv_path);
		void getKinematicsSettingDataCSV(std::string csv_path);
	};

}