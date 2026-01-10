#pragma once

#ifdef EMSCRIPTEN
// Console looks like ass in emscripten with this.
constexpr auto TTY_RESET = "";
constexpr auto TTY_RED = "";
constexpr auto TTY_GREEN = "";
constexpr auto TTY_YELLOW = "";
constexpr auto TTY_BLUE = "";
#else
constexpr auto TTY_RESET = "\033[0m";
constexpr auto TTY_RED = "\033[31m";
constexpr auto TTY_GREEN = "\033[32m";
constexpr auto TTY_YELLOW = "\033[33m";
constexpr auto TTY_BLUE = "\033[34m";
#endif

#define UNUSED(x) ((void)(x))

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>