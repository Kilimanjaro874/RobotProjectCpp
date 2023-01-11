#include "gm_actor.h"

void tol::Actor::update(float delta_time) {
	coordinate_.update(delta_time);
}

void tol::Actor::render(dxe::Camera* camera) {
	assemble_->render(camera);
}

tol::Actor* tol::Actor::Create(AssemRepo* assem_repo) {
	Actor* act = new Actor();
	act->coordinate_.setCoordinate();
	act->assemble_ = assem_repo->getAssemble(200, "", true, 1.0);
	return act;
}
