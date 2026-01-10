#pragma once

#include "../../core/asset.hpp"
#include "../../gl/defines.hpp"

#include <fstream>

namespace NEX::GL
{
    struct VertexShader
    {
        GLuint id = 0;

        VertexShader(std::ifstream&);
        VertexShader(std::string);
        ~VertexShader();
    };

    struct FragmentShader
    {
        GLuint id = 0;

        FragmentShader(std::ifstream&);
        FragmentShader(std::string);
        ~FragmentShader();
    };

    struct Shader : public Core::IAsset
    {
        GLuint id = 0;

        void use();
        Shader(std::string, std::string);
        Shader(const VertexShader&, const FragmentShader&);

        void set_uniform(std::string const name, int value);
        void set_uniform(std::string const name, float value);
        void set_uniform(std::string const name, double value);
        ~Shader();
    };
}; // namespace NEX::GL
