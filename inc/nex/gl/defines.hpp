#pragma once

#include <SDL3/SDL.h>

#if defined(_WIN32)
#include <glad/glad.h>
// #elif defined(__psp2__)
// #include <vitaGL.h>
#else
#include <SDL3/SDL_opengles2.h>
#endif

#define GEN_AND_SEND_VBO(vbo, verts, usage)                                                                                                          \
    glGenBuffers(1, &vbo);                                                                                                                           \
    glBindBuffer(GL_ARRAY_BUFFER, vbo);                                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, usage);

namespace NEX::GL
{
    extern float FULLSCREEN_RECT2D[16];

    void clear_color(SDL_FColor);
    void set_view(SDL_Rect);
    void set_view_letterbox(SDL_Point, SDL_Point);
} // namespace NEX::GL
