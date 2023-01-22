#include "gm_controller.h"

std::shared_ptr<tol::Commando> tol::InputHandler::handleInput(float delta_time)
{
    if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
        return MoveHorizontal;
    }

    return nullptr;
}
