#include "prefabs/sprite.hpp"

using namespace NEX::Prefabs;

Sprite::Sprite(VertexData data[4])
{
    SDL_memcpy(this->data, data, 4 * sizeof(VertexData));
    GEN_AND_SEND_VBO(vbo, this->data, GL_DYNAMIC_DRAW);
}

Sprite::~Sprite()
{
    if(this->vbo)
        glDeleteBuffers(1, &this->vbo);
}

void Sprite::draw(GL::Shader& shader, GLenum mode)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glUniform4f(glGetUniformLocation(shader.id, "u_color"), this->color.r, this->color.g, this->color.b, this->color.a);

    GLint a_position = glGetAttribLocation(shader.id, "a_position");
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    GLint a_texcoord = glGetAttribLocation(shader.id, "a_texture_coords");
    glEnableVertexAttribArray(a_texcoord);
    glVertexAttribPointer(a_texcoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glDrawArrays(mode, 0, 4);
}

void Sprite::refresh(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferSubData(this->vbo, 0, 4 * sizeof(VertexData), &this->data);
}