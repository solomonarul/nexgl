#include "gl/window.hpp"

#include "core/defines.hpp"

#include <cstddef>
#include <cstdlib>

using namespace NEX::GL;

static size_t window_count = 0;

static bool init_sdl_video()
{
    bool status = SDL_Init(SDL_INIT_VIDEO);

    // Request GLES2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    return status;
}

static void free_sdl_video()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

Window::Window(WindowConfig cfg)
{
    if (window_count == 0 && !init_sdl_video())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Could not init SDL3!", SDL_GetError(), NULL);
        exit(-1);
    }

    this->sdl = SDL_CreateWindow(cfg.title.c_str(), cfg.w, cfg.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!this->sdl)
    {
        if (window_count == 0)
            free_sdl_video();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Could not init SDL3 window!", SDL_GetError(), NULL);
        exit(-1);
    }

    this->gl = SDL_GL_CreateContext(this->sdl);
    if (!this->gl)
    {
        SDL_DestroyWindow(this->sdl), this->sdl = nullptr;
        if (window_count == 0)
            free_sdl_video();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Could not initialize GLES 2.0 context!", SDL_GetError(), NULL);
        exit(-1);
    }

    SDL_GL_MakeCurrent(this->sdl, this->gl);

#ifdef _WIN32
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Could not initialize GLES 2.0!", SDL_GetError(), NULL);
        exit(-1);
    }
#endif

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->w = cfg.w;
    this->h = cfg.h;

    window_count++;

    SDL_Log("%s[INFO] Created SDL window (title: %s) (w: %ld) (h: %ld)", TTY_BLUE, cfg.title.c_str(), cfg.w, cfg.h);
    SDL_Log("[INFO] GLES 2.0 context data:%s\n", TTY_RESET);
    SDL_Log("Vendor   : %s", glGetString(GL_VENDOR));
    SDL_Log("Renderer : %s", glGetString(GL_RENDERER));
    SDL_Log("Version  : %s", glGetString(GL_VERSION));
    SDL_Log("GLSL     : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Window::use()
{
    SDL_GL_MakeCurrent(this->sdl, this->gl);
}

void Window::vsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable ? 1 : 0);
}

void Window::set_fullscreen(bool status)
{
#if defined(__psp2__)
    return;
#else
    SDL_SetWindowFullscreen(this->sdl, status);
    this->fullscreen = status;
    SDL_Log("%s[INFO] Window fullscreen mode set to %d.%s\n", TTY_BLUE, status, TTY_RESET);
#endif
}

void Window::swap()
{
    SDL_GL_SwapWindow(this->sdl);
}

Window::~Window()
{
    window_count--;

    if (this->gl)
        SDL_GL_DestroyContext(this->gl), this->gl = nullptr;

    if (this->sdl)
        SDL_DestroyWindow(this->sdl), this->sdl = nullptr;

    if (window_count == 0)
        free_sdl_video();
}
