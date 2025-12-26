#include "gl/assets/shader.hpp"

#include "core/defines.hpp"
#include "core/file.hpp"

#include <iostream>

using namespace NEX::GL;

static GLchar shader_log[512];

VertexShader::VertexShader(std::ifstream &file) : VertexShader(Core::read_file_contents(file))
{
}

VertexShader::VertexShader(std::string code)
{
    this->id = glCreateShader(GL_VERTEX_SHADER);
    const char *code_str = code.c_str();
    glShaderSource(this->id, 1, &code_str, NULL);
    glCompileShader(this->id);

    static GLint success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->id, sizeof(shader_log), NULL, shader_log);
        std::cerr << TTY_RED << "[EROR] Vertex shader compilation error:\n" << TTY_YELLOW << shader_log << "\n" << TTY_RESET;
        this->id = 0;
        return;
    }

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Loaded vertex shader (id: " << this->id << ")\n" << TTY_RESET;
#endif
}

VertexShader::~VertexShader()
{
    if (this->id != 0)
    {
        glDeleteShader(this->id);
#ifdef ASSET_LOAD_LOG
        std::cout << TTY_BLUE << "[INFO] Destroyed vertex shader (id: " << this->id << ")\n" << TTY_RESET;
#endif
        this->id = 0;
    }
    else
        std::cerr << TTY_YELLOW << "[WARN] Tried to destroy inexistent vertex shader.\n" << TTY_RESET;
}

FragmentShader::FragmentShader(std::ifstream &file) : FragmentShader(Core::read_file_contents(file))
{
}

FragmentShader::FragmentShader(std::string code)
{
    this->id = glCreateShader(GL_FRAGMENT_SHADER);
    const char *code_str = code.c_str();
    glShaderSource(this->id, 1, &code_str, NULL);
    glCompileShader(this->id);

    static GLint success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->id, sizeof(shader_log), NULL, shader_log);
        std::cerr << TTY_RED << "[EROR] Fragment shader compilation error:\n" << TTY_YELLOW << shader_log << "\n" << TTY_RESET;
        this->id = 0;
        return;
    }

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Loaded fragment shader (id: " << this->id << ")\n" << TTY_RESET;
#endif
}

FragmentShader::~FragmentShader()
{
    if (this->id != 0)
    {
        glDeleteShader(this->id);
#ifdef ASSET_LOAD_LOG
        std::cout << TTY_BLUE << "[INFO] Destroyed fragment shader (id: " << this->id << ")\n" << TTY_RESET;
#endif
        this->id = 0;
    }
    else
        std::cerr << TTY_YELLOW << "[WARN] Tried to destroy inexistent fragment shader.\n" << TTY_RESET;
}

inline void init_from_shaders(Shader &self, const VertexShader &vert, const FragmentShader &frag)
{
    self.id = glCreateProgram();
    if (vert.id != 0)
        glAttachShader(self.id, vert.id);

    if (frag.id != 0)
        glAttachShader(self.id, frag.id);

    glLinkProgram(self.id);

    static GLint linkStatus = 0;
    glGetProgramiv(self.id, GL_LINK_STATUS, &linkStatus);

    if (!linkStatus)
    {
        glGetProgramInfoLog(self.id, sizeof(shader_log), NULL, shader_log);
        std::cerr << TTY_RED << "[EROR] General shader linking error:\n" << TTY_YELLOW << shader_log << "\n" << TTY_RESET;
        self.id = 0;
        return;
    }

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Loaded full shader (id: " << self.id << ") made out of vertex (id: " << vert.id
              << ") and fragment (id: " << frag.id << ") shaders.\n"
              << TTY_RESET;
#endif
}

Shader::Shader(const VertexShader &vert, const FragmentShader &frag)
{
    init_from_shaders(*this, vert, frag);
}

Shader::Shader(std::string vertex_path, std::string fragment_path)
{
    std::ifstream vfile(vertex_path);
    if (!vfile.is_open())
    {
        std::cerr << TTY_YELLOW << "[WARN] Could not open vertex shader (path: " << vertex_path << "), aborting compilation...\n" << TTY_RESET;
        return;
    }

    std::ifstream ffile(fragment_path);
    if (!ffile.is_open())
    {
        std::cerr << TTY_YELLOW << "[WARN] Could not open fragment shader (path: " << vertex_path << "), aborting compilation...\n" << TTY_RESET;
        return;
    }

    GL::VertexShader vs(vfile);
    GL::FragmentShader fs(ffile);

    init_from_shaders(*this, vs, fs);

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Shader (id: " << this->id << ") was loaded from vertex shader (path: " << vertex_path
              << "), fragment shader (path: " << fragment_path << ")\n"
              << TTY_RESET;
#endif
}

void Shader::use()
{
    glUseProgram(this->id);
}

void Shader::setUniform(std::string const name, int value)
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setUniform(std::string const name, float value)
{
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

Shader::~Shader()
{
    if (this->id != 0)
    {
        glDeleteProgram(id);
        this->id = 0;
#ifdef ASSET_LOAD_LOG
        std::cout << TTY_BLUE << "[INFO] Destroyed full shader (id: " << this->id << ")\n" << TTY_RESET;
#endif
    }
    else
        std::cerr << TTY_YELLOW << "[WARN] Tried to destroy inexistent full shader.\n" << TTY_RESET;
}
