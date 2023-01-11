#pragma once
#include <memory>
#include "gm_object.h"
#include "gm_assemble_repository.h"

namespace tol {
	class Actor : public Object {
	public:
		Actor() {}
		~Actor() {};

	public:
		void update(float delta_time) override;
		void render(dxe::Camera* camera) override;
		static Actor* Create(AssemRepo* assem_repo);
	};

}