#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengles2.h>

#define GEN_AND_SEND_VBO(vbo, verts, usage)                                                                                                          \
    glGenBuffers(1, &vbo);                                                                                                                           \
    glBindBuffer(GL_ARRAY_BUFFER, vbo);                                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, usage);

namespace NEX::GL
{
    extern float FULLSCREEN_RECT2D[16];

    void set_view(SDL_Rect);
    void set_view_letterbox(SDL_Point, SDL_Point);
}
