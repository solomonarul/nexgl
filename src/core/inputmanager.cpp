#include "core/inputmanager.hpp"

using namespace NEX::Core;

void InputManager::set_letterbox(SDL_Rect letterbox)
{
    auto& self = InputManager::self();
    self.r_letterbox = letterbox;
}

void InputManager::handle_event(SDL_Event& event)
{
    auto& self = InputManager::self();
    switch (event.type)
    {
        case SDL_EVENT_MOUSE_MOTION:
            if (self.r_letterbox.w != 0)
            {
                if (event.pmotion.x < self.r_letterbox.x || event.pmotion.x > self.r_letterbox.x + self.r_letterbox.w ||
                    event.pmotion.y < self.r_letterbox.y || event.pmotion.y > self.r_letterbox.y + self.r_letterbox.h)
                    break;
                self.m_data.x = event.pmotion.x - self.r_letterbox.x;
                self.m_data.y = event.pmotion.y - self.r_letterbox.y;
            }
            else
            {
                self.m_data.x = event.pmotion.x;
                self.m_data.y = event.pmotion.y;
            }
            break;
    }
}

InputManager::MouseData InputManager::get_mouse_data()
{
    return InputManager::self().m_data;
}

InputManager& InputManager::self()
{
    static InputManager self;
    return self;
}
