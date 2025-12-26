#include "core/timer.hpp"

#include <cmath>

using namespace NEX::Core;

Timer::Timer(double rate) : rate(rate)
{
}

void Timer::update(double dt)
{
    this->value += dt;
    if (this->value > this->rate)
    {
        this->ok = true;
        this->value = std::fmod(this->value, this->rate);
    }
}

bool Timer::has_ticked(void)
{
    if (!this->ok)
        return false;
    this->ok = false;
    return true;
}
