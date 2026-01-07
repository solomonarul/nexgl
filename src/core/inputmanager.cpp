#include "core/inputmanager.hpp"

using namespace NEX::Core;

SDL_FPoint InputManager::MouseData::get_coords(void)
{
    return SDL_FPoint{this->x, this->y};
}

SDL_FPoint InputManager::MouseData::get_coords_normalized(void)
{
    return SDL_FPoint{(this->x - 0.5f) * 2, -(this->y - 0.5f) * 2};
}

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

                self.m_data.x = (event.pmotion.x - self.r_letterbox.x) * 1.0 / self.r_letterbox.w;
                self.m_data.y = (event.pmotion.y - self.r_letterbox.y) * 1.0 / self.r_letterbox.h;
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

bool InputManager::MouseData::is_in_rect(SDL_FRect rect)
{
    SDL_FPoint coords = InputManager::self().m_data.get_coords();
    return SDL_PointInRectFloat(&coords, &rect);
}

bool InputManager::MouseData::is_in_normalized_rect(SDL_FRect rect)
{
    SDL_FPoint coords = InputManager::self().m_data.get_coords_normalized();
    return SDL_PointInRectFloat(&coords, &rect);
}

InputManager& InputManager::self()
{
    static InputManager self;
    return self;
}
