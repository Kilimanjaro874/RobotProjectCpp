#include "gm_input_handler.h"

void tol::InputHandler::init()
{
	button_right_ = std::make_unique<MoveRight>();
	button_left_ = std::make_unique<MoveLeft>();
	button_up_ = std::make_unique<MoveFront>();
	button_down_ = std::make_unique<MoveBack>();
}

void tol::InputHandler::update() {

}

std::shared_ptr<tol::Commando> tol::InputHandler::handleInput()
{
	// Left Stick or KB move 
	tnl::Vector3 pad_left_stick = tnl::Input::GetLeftStick();
	if (pad_left_stick.length() > 0.10f) {

	}
	else {
		if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			return button_right_;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			return button_left_;
		}
	}

	return nullptr;
}
