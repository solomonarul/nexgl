#include "gl/defines.hpp"

#include <algorithm>

#include "core/inputmanager.hpp"

using namespace NEX::GL;

// clang-format off
float NEX::GL::FULLSCREEN_RECT2D[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f,
};
// clang-format on

void NEX::GL::set_view(SDL_Rect rect)
{
    glViewport(rect.x, rect.y, rect.w, rect.h);
    Core::InputManager::set_letterbox(rect);
}

void NEX::GL::set_view_letterbox(SDL_Point size, SDL_Point original)
{
    double scale = std::min(size.x * 1.0 / original.x, size.y * 1.0 / original.y);
    int s_w = (size.x - original.x * scale) / 2, s_h = (size.y - original.y * scale) / 2;
    NEX::GL::set_view(SDL_Rect{s_w, s_h, size.x - 2 * s_w, size.y - 2 * s_h});
}