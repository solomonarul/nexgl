#pragma once

#define GEN_AND_SEND_VBO(vbo, verts, usage)                                                                                                          \
    glGenBuffers(1, &vbo);                                                                                                                           \
    glBindBuffer(GL_ARRAY_BUFFER, vbo);                                                                                                              \
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, usage);

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengles2.h>
