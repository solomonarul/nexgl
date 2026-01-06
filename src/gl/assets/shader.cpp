#include "gl/assets/shader.hpp"

#include "core/defines.hpp"
#include "core/file.hpp"

using namespace NEX::GL;

static GLchar shader_log[512];

VertexShader::VertexShader(std::ifstream& file) : VertexShader(Core::read_file_contents(file)) {}

VertexShader::VertexShader(std::string code)
{
    this->id = glCreateShader(GL_VERTEX_SHADER);
    const char* code_str = code.c_str();
    glShaderSource(this->id, 1, &code_str, NULL);
    glCompileShader(this->id);

    static GLint success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->id, sizeof(shader_log), NULL, shader_log);
        SDL_Log("%s[EROR] Vertex shader compilation error:%s%s%s\n", TTY_RED, TTY_YELLOW, shader_log, TTY_RESET);
        this->id = 0;
        return;
    }
}

VertexShader::~VertexShader()
{
    if (this->id != 0)
    {
        glDeleteShader(this->id);
        this->id = 0;
    }
    else
        SDL_Log("%s[WARN] Tried to destroy inexistent vertex shader.%s\n", TTY_YELLOW, TTY_RESET);
}

FragmentShader::FragmentShader(std::ifstream& file) : FragmentShader(Core::read_file_contents(file)) {}

FragmentShader::FragmentShader(std::string code)
{
    this->id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* code_str = code.c_str();
    glShaderSource(this->id, 1, &code_str, NULL);
    glCompileShader(this->id);

    static GLint success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(this->id, sizeof(shader_log), NULL, shader_log);
        SDL_Log("%s[EROR] Fragment shader compilation error:%s%s%s\n", TTY_RED, TTY_YELLOW, shader_log, TTY_RESET);
        this->id = 0;
        return;
    }
}

FragmentShader::~FragmentShader()
{
    if (this->id != 0)
    {
        glDeleteShader(this->id);
        this->id = 0;
    }
    else
        SDL_Log("%s[WARN] Tried to destroy inexistent fragment shader.%s\n", TTY_YELLOW, TTY_RESET);
}

inline void init_from_shaders(Shader& self, const VertexShader& vert, const FragmentShader& frag)
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
        SDL_Log("%s[EROR] General shader linking error:%s%s%s\n", TTY_RED, TTY_YELLOW, shader_log, TTY_RESET);
        self.id = 0;
        return;
    }
}

Shader::Shader(const VertexShader& vert, const FragmentShader& frag)
{
    init_from_shaders(*this, vert, frag);
}

Shader::Shader(std::string vertex_path, std::string fragment_path)
{
    std::ifstream vfile(vertex_path);
    if (!vfile.is_open())
    {
        SDL_Log("%s[WARN] Could not open vertex shader (path: %s), aborting compilation...%s\n", TTY_YELLOW, vertex_path, TTY_RESET);
        return;
    }

    std::ifstream ffile(fragment_path);
    if (!ffile.is_open())
    {
        SDL_Log("%s[WARN] Could not open fragment shader (path: %s), aborting compilation...%s\n", TTY_YELLOW, fragment_path, TTY_RESET);
        return;
    }

    GL::VertexShader vs(vfile);
    GL::FragmentShader fs(ffile);

    init_from_shaders(*this, vs, fs);
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
    }
    else
        SDL_Log("%s[WARN] Tried to destroy inexistent full shader.%s\n", TTY_YELLOW, TTY_RESET);
}
