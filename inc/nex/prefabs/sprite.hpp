#pragma once

#include "../gl/assets/texture.hpp"
#include "vertexdata.hpp"

namespace NEX::Prefabs
{
    struct Sprite
    {
        GLuint vbo = 0;
        VertexData data[4];
        SDL_FColor color = {1, 1, 1, 1};

        // 0 1
        // 3 2
        // Order of vertices as per GL_TRIANGLE_STRIP
        Sprite(VertexData[4] = (VertexData*)GL::FULLSCREEN_RECT2D);
        ~Sprite();

        void draw(GL::Shader&);

        // Call this after updating data to send to the GPU as well.
        void refresh(void);
        void refresh_from_rect(SDL_FRect);
    };
}; // namespace NEX::Prefabs