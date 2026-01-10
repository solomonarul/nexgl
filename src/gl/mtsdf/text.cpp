#include "gl/mtsdf/text.hpp"

using namespace NEX::GL;

inline void gen_and_upload_vertices(MTSDF::Text& self)
{
    self.vertexData.clear();

    // Calculate maximum sizes.
    float t_c_w = 0, t_w = 0, t_h = 0, lines = 1;
    for (char c : self.text)
    {
        auto it = self.font->glyphs.find(c);
        if (it == self.font->glyphs.end())
            continue;

        t_c_w += self.s * it->second.advance * self.s_x;
        if (c == '\n')
            lines++, t_w = (t_c_w > t_w) ? t_c_w : t_w, t_c_w = 0;
    }
    t_w = (t_c_w > t_w) ? t_c_w : t_w;
    t_h = self.s * lines * self.s_y;

    float pen_x = self.x + t_w * self.o_x;
    float pen_y = self.y + t_h * self.o_y;

    for (char c : self.text)
    {
        if (c == '\n')
        {
            pen_x = self.x + t_w * self.o_x, pen_y -= self.s * self.s_y;
            continue;
        }

        auto it = self.font->glyphs.find(c);
        if (it == self.font->glyphs.end())
            continue;

        SDL_FRect& atlas = it->second.atlas;
        float& u0 = atlas.x;
        float& v0 = atlas.y;
        float& u1 = atlas.w;
        float& v1 = atlas.h;

        SDL_FRect quad = it->second.quad;
        quad.x *= self.s * self.s_x;
        quad.y *= self.s * self.s_y;
        quad.w *= self.s * self.s_x;
        quad.h *= self.s * self.s_y;

        // DO not send whitespaces. They must update the cursor but do not generate triangles for them.
        if (!std::isspace(c))
        {
            const float X1 = pen_x + quad.x;
            const float Y1 = pen_y + quad.y;
            const float X2 = pen_x + quad.w;
            const float Y2 = pen_y + quad.h;

            // clang-format off
            self.vertexData.insert(self.vertexData.end(), {
                X1, Y1, u0, v1,  X2, Y1, u1, v1,  X2, Y2, u1, v0,
                X1, Y1, u0, v1,  X2, Y2, u1, v0,  X1, Y2, u0, v0,
            });
            self.box.x = (self.box.x < X1) ? self.box.x : X1;
            self.box.y = (self.box.y < Y1) ? self.box.y : Y1;
            self.box.w = (self.box.w > X2) ? self.box.x : X2;
            self.box.h = (self.box.h > Y2) ? self.box.y : Y2;
            // clang-format on
        }

        pen_x += self.s * it->second.advance * self.s_x;
    }

    glBindBuffer(GL_ARRAY_BUFFER, self.vbo);
    glBufferData(GL_ARRAY_BUFFER, self.vertexData.size() * sizeof(float), self.vertexData.data(), GL_DYNAMIC_DRAW);
}

MTSDF::Text::Text(std::shared_ptr<Font> f, const std::string& str) : font(std::move(f)), text(str)
{
    glGenBuffers(1, &this->vbo);
}

MTSDF::Text::~Text()
{
    if (this->vbo)
        glDeleteBuffers(1, &this->vbo), this->vbo = 0;
}

SDL_FRect MTSDF::Text::get_bounding_box(void)
{
    return SDL_FRect{.x = this->box.x, .y = this->box.y, .w = this->box.w - this->box.x, .h = this->box.h - this->box.y};
}

void MTSDF::Text::draw(std::unique_ptr<Shader>& shader)
{
    shader->use();
    shader->set_uniform("u_texture", 0);
    glUniform4f(glGetUniformLocation(shader->id, "u_color"), this->color.r / 255.0, this->color.g / 255.0, this->color.b / 255.0,
                this->color.a / 255.0);

    this->font->texture->activate(GL_TEXTURE0);

    gen_and_upload_vertices(*this); // TODO: let's not do this every frame.

    auto a_position = glGetAttribLocation(shader->id, "a_position");
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    auto a_texture_coords = glGetAttribLocation(shader->id, "a_texture_coords");
    glEnableVertexAttribArray(a_texture_coords);
    glVertexAttribPointer(a_texture_coords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 4);
}
