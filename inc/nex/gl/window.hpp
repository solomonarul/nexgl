#pragma once

#include <string>
#include "defines.hpp"

namespace NEX::GL
{
    struct WindowConfig
    {
        std::string title;
        int w, h;
    };

    struct Window
    {
        SDL_Window* sdl = nullptr;
        SDL_GLContext gl = nullptr;
#if defined(__psp2__)
        bool fullscreen = true;
#else
        bool fullscreen = false;
#endif
        int w, h;

        void use();
        void swap();
        void vsync(bool);
        void set_fullscreen(bool);
        Window(WindowConfig);
        ~Window();
    };
}; // namespace NEX::GL
