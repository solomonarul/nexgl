#pragma once

#include <string>
#include "defines.hpp"

namespace NEX::GL
{
    struct WindowConfig
    {
        std::string title;
        size_t w, h;
    };

    struct Window
    {
        SDL_Window* sdl = nullptr;
        SDL_GLContext gl = nullptr;
        int w, h;

        void use();
        void swap();
        void vsync(bool);
        Window(WindowConfig);
        ~Window();
    };
}; // namespace NEX::GL
