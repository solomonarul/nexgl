#include "prefabs/spriteatlas.hpp"

using namespace NEX::Prefabs;

SpriteAtlas::SpriteAtlas(size_t w, size_t h)
{
    this->w = w;
    this->h = h;
}

SpriteAtlas::~SpriteAtlas()
{
    if (this->vbo)
        glDeleteBuffers(1, &this->vbo);
}

void SpriteAtlas::push_sprite(size_t index, SDL_FRect target_rect)
{
    // clang-format off
    index %= this->w * this->h;
    const auto pos_x = index % this->w;
    const auto pos_y = index / this->w;

    const auto size_x = 1.0f / this->w;
    const auto size_y = 1.0f / this->h;

    const SDL_FRect uv_rect = {
        .x = size_x * pos_x,
        .y = size_y * pos_y,
        .w = size_x,
        .h = size_y
    };

    this->data.push_back(VertexData{
        .x = target_rect.x,
        .y = target_rect.y,
        .u = uv_rect.x,
        .v = uv_rect.y
    });

    this->data.push_back(VertexData{
        .x = target_rect.x + target_rect.w,
        .y = target_rect.y,
        .u = uv_rect.x + uv_rect.w,
        .v = uv_rect.y
    });

    this->data.push_back(VertexData{
        .x = target_rect.x,
        .y = target_rect.y + target_rect.h,
        .u = uv_rect.x,
        .v = uv_rect.y + uv_rect.h
    });

    this->data.push_back(VertexData{
        .x = target_rect.x,
        .y = target_rect.y + target_rect.h,
        .u = uv_rect.x,
        .v = uv_rect.y + uv_rect.h
    });

    this->data.push_back(VertexData{
        .x = target_rect.x + target_rect.w,
        .y = target_rect.y,
        .u = uv_rect.x + uv_rect.w,
        .v = uv_rect.y
    });

    this->data.push_back(VertexData{
        .x = target_rect.x + target_rect.w,
        .y = target_rect.y + target_rect.h,
        .u = uv_rect.x + uv_rect.w,
        .v = uv_rect.y + uv_rect.h
    });
    //clang-format on
}

void SpriteAtlas::draw(GL::Shader& shader)
{
    //clang-format off
    if(!this->vbo)
        glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        this->data.size() * sizeof(VertexData),
        this->data.data(),
        GL_DYNAMIC_DRAW
    );
    //clang-format on

    glUniform4f(glGetUniformLocation(shader.id, "u_color"), this->color.r, this->color.g, this->color.b, this->color.a);

    GLint a_position = glGetAttribLocation(shader.id, "a_position");
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

    GLint a_texcoord = glGetAttribLocation(shader.id, "a_texture_coords");
    glEnableVertexAttribArray(a_texcoord);
    glVertexAttribPointer(a_texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(2 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, this->data.size());

    this->data.clear();
}
