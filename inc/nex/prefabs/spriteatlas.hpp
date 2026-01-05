#pragma once

#include <vector>

#include "../gl/assets/texture.hpp"
#include "vertexdata.hpp"

using namespace NEX::Prefabs;
using namespace NEX::GL;

namespace NEX::Prefabs
{
    struct SpriteAtlas
    {
        GLuint vbo = 0;
        size_t w, h;
        SDL_FColor color = {1, 1, 1, 1};

        SpriteAtlas(size_t = 1, size_t = 1);
        ~SpriteAtlas();
        void push_sprite(size_t, SDL_FRect);
        void draw(GL::Shader&);

      private:
        std::vector<VertexData> data;
    };
}; // namespace NEX::Prefabs