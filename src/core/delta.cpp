#include "core/delta.hpp"

#include "core/defines.hpp"

using namespace NEX::Core;

DeltaTimer::DeltaTimer() : last(SDL_GetPerformanceCounter())
{
}

double DeltaTimer::tick()
{
    uint64_t now = SDL_GetPerformanceCounter();
    double dt = double(now - last) / float(SDL_GetPerformanceFrequency());
    last = now;
    return dt;
}
