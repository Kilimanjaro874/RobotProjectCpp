#pragma once
#include "gm_model.h"

class MdlLink : public Model {
	// ----- 3�����N�̃A�[�����쐬 ------ //
	enum {
		e_link01,
		e_link02,
		e_link03
	};

	void update(float delta_time) override;
	static MdlLink* Create();
};