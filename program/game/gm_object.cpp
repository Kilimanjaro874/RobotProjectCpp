#include "gm_object.h"


void tol::Object::update(float delta_time) {
	coordinate_.update(delta_time);

	//assemble_->update(delta_time);
}

void tol::Object::render(dxe::Camera* camera) {
	assemble_->render(camera);
}