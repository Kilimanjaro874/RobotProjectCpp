#include "gm_link.h"

MdlLink* MdlLink::Create() {
	MdlLink* mdl = new MdlLink();

	return mdl;
}

void MdlLink::update(float delta_time) {
	Model::update(delta_time);
}