#pragma once

#include <SDL3/SDL_events.h>

namespace NEX::Core
{
    struct InputManager
    {
        struct MouseData
        {
            float x, y;
        };

        SDL_Rect r_letterbox = {0, 0, 0, 0};

        static void set_letterbox(SDL_Rect);
        static void handle_event(SDL_Event&);

        // If there is a letterbox set, this uses normalized coords, otherwise it's normal pixels.
        static MouseData get_mouse_data();

      private:
        MouseData m_data;

        InputManager() = default;
        static InputManager& self();
    };
}; // namespace NEX::Core
